#include "ParticleSystem.h"

#include "DirectXTex.h"
#include "Game.h"
#include "Camera.h"

ParticleSystem::~ParticleSystem()
{
	HMVertexShader->Release();
	HMVertexShaderByteCode->Release();

	DepthView->Release();
	heightMap->Release();
	HeightView->Release();

	HMSamplerState->Release();
	heightMapBuf->Release();

	indirectBuffer->Release();
	constBuffer->Release();
	emitterBuffer->Release();

	particlesBuffer->Release();
	particlesUAV->Release();
	particlesSRV->Release();

	deadListBuffer->Release();
	deadListUAV->Release();

	sortedListBuffer->Release();
	sortedListUAV->Release();
	sortedListSRV->Release();

	textureView->Release();
	samplerState->Release();

	vertexShader->Release();
	vertexShaderByteCode->Release();

	pixelShader->Release();
	pixelShaderByteCode->Release();

	geometryShader->Release();
	geometryShaderByteCode->Release();

	sortingCBuffer->Release();
	sortShader->Release();
	sortShaderByteCode->Release();

	transposeShader->Release();
	transposeShaderByteCode->Release();

	emitShader->Release();
	emitShaderByteCode->Release();

	simulateShader->Release();
	simulateShaderByteCode->Release();
}

void ParticleSystem::Initialize()
{
	LoadTexture();

	InitResouces();

	LoadShaders();

	InitHeightMap();
}

void ParticleSystem::Update(float deltaTime)
{
	Emit(10);
	UINT x = GetDispatchDim(maxNumberOfParticles);
	UINT y = x;
	constBufferData.projection = Game::GetCamera()->projection;
	constBufferData.view = Game::GetCamera()->view;
	emitter.deltaTime = deltaTime;
	emitter.numGroups = x;
	UpdateConstBuffer();

	ID3D11UnorderedAccessView* UAVs[] = { sortedListUAV, particlesUAV, nullptr, deadListUAV };
	UINT counters[] = { 0, -1, -1, -1 }; 
	Context->CSSetUnorderedAccessViews(0, 4, UAVs, counters);
	Context->CSSetConstantBuffers(0, 1, &constBuffer);
	Context->CSSetConstantBuffers(4, 1, &emitterBuffer);

	Context->CSSetShader(simulateShader, nullptr, 0);
	
	UINT z = 1;

	Context->Dispatch(x, y, z);

	SortParticles();

	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ParticleSystem::Emit(int nParticles)
{
	UINT x = GetDispatchDim(std::min(nParticles, maxSpawnRate));
	UINT y = x;
	emitter.numGroups = x;
	emitter.emitCount = std::min(nParticles, maxSpawnRate);
	UpdateConstBuffer();

	ID3D11UnorderedAccessView* UAVs[] = { sortedListUAV, particlesUAV, deadListUAV, nullptr};
	UINT counters[] = {0, -1, -1, -1 };
	Context->CSSetUnorderedAccessViews(0, 4, UAVs, counters);

	Context->CSSetConstantBuffers(0, 1, &constBuffer);
	Context->CSSetConstantBuffers(4, 1, &emitterBuffer);

	Context->CSSetShader(emitShader, nullptr, 0);

	//TODO Correct x y z
	UINT z = 1;

	Context->Dispatch(x, y, z);
}

void ParticleSystem::Render()
{
	ID3D11UnorderedAccessView* UAVs[] = { nullptr, nullptr, nullptr, nullptr };
	UINT counters[] = { -1, -1, -1, -1 };
	Context->CSSetUnorderedAccessViews(0, 4, UAVs, counters);

	Context->CSSetShader(nullptr, nullptr, 0);
	//return;

	Context->IASetInputLayout(nullptr);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	Context->VSSetShader(vertexShader, nullptr, 0);
	Context->GSSetShader(geometryShader, nullptr, 0);
	Context->PSSetShader(pixelShader, nullptr, 0);
	
	Context->VSSetConstantBuffers(0, 1, &constBuffer);

	ID3D11ShaderResourceView* SRVs[] = {particlesSRV, sortedListSRV};
	Context->VSSetShaderResources(1, 2, SRVs);

	Context->GSSetConstantBuffers(0, 1, &constBuffer);

	Context->PSSetShaderResources(0, 1, &textureView);

	Context->CopyStructureCount(indirectBuffer, 0, sortedListUAV);

	Context->DrawInstancedIndirect(indirectBuffer, 0);

	Context->GSSetShader(nullptr, nullptr, 0); 
	ID3D11ShaderResourceView* nullSRVs[] = { nullptr, nullptr };
	Context->VSSetShaderResources(1, 2, nullSRVs);
}

void ParticleSystem::RenderHeightMap()
{
	Vector4 pos[2] = { emitter.minSpawnPos, emitter.maxSpawnPos};

	pos[0] = Vector4::Transform(pos[0], World);
	pos[1] = Vector4::Transform(pos[1], World);

	Vector4 center4 = (pos[0]+ pos[1]) / 2;
	Vector3 center = Vector3(center4.x, center4.y, center4.z);

	Vector3 rainDir = Vector3(emitter.force.x, emitter.force.y, emitter.force.z);
	rainDir.Normalize();

	auto view = Matrix::CreateLookAt(center, center + rainDir, Vector3::Up);

	std::vector<Vector3> corners;
	corners.reserve(8);
	for (int i = 0; i < 8; i++) {
		corners.push_back(Vector3(pos[(i & 1)].x, pos[((i & 2) >> 1)].y, pos[((i & 4) >> 2)].z));
		//std::cout << (i & 1) << " " << ((i & 2) >> 1) << " " << ((i & 4) >> 2) << "\n";
	}

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		auto trf = Vector3::Transform(v, view);
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	constexpr float zMult = 10.0f;

	minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
	maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;
	heightMapData.viewProjection = view * Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto res = Context->Map(heightMapBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &heightMapData, sizeof(heightMapBuf));
	Context->Unmap(heightMapBuf, 0);

	Context->VSSetConstantBuffers(5, 1, &heightMapBuf);

	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	Context->VSSetShaderResources(3, 1, nullsrv);

	ID3D11RenderTargetView* nullrtv[] = { nullptr,nullptr,nullptr,nullptr, nullptr,nullptr,nullptr,nullptr };
	Context->OMSetRenderTargets(8, nullrtv, DepthView);

	Context->RSSetViewports(1, viewport.Get11());
	Context->VSSetShader(HMVertexShader, nullptr, 0);
	Context->GSSetShader(nullptr, nullptr, 0);
	Context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSystem::BindHeightMap()
{
	Context->CSSetShaderResources(3, 1, &HeightView);
	Context->CSSetSamplers(1, 1, &HMSamplerState);
}

void ParticleSystem::InitResouces()
{
	D3D11_BUFFER_DESC particlesBuffDesc = {};
	particlesBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	particlesBuffDesc.ByteWidth = sizeof(Particle) * maxNumberOfParticles;
	particlesBuffDesc.CPUAccessFlags = 0;
	particlesBuffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	particlesBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	particlesBuffDesc.StructureByteStride = sizeof(Particle);

	Device->CreateBuffer(&particlesBuffDesc, nullptr, &particlesBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC pUAVDesc = {};
	pUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	pUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	pUAVDesc.Buffer.NumElements = maxNumberOfParticles;

	Device->CreateUnorderedAccessView(particlesBuffer, &pUAVDesc, &particlesUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC pSRVDesc = {};
	pSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	/*pSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	pSRVDesc.Buffer.NumElements = maxNumberOfParticles;
	pSRVDesc.Buffer.ElementWidth = sizeof(SortingData);*/
	pSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	pSRVDesc.BufferEx.FirstElement = 0;
	pSRVDesc.BufferEx.NumElements = maxNumberOfParticles;

	Device->CreateShaderResourceView(particlesBuffer, &pSRVDesc, &particlesSRV);


	D3D11_BUFFER_DESC dlBDesc = {};
	dlBDesc.Usage = D3D11_USAGE_DEFAULT;
	dlBDesc.ByteWidth = sizeof(UINT) * maxNumberOfParticles;
	dlBDesc.CPUAccessFlags = 0;
	dlBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	dlBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	dlBDesc.StructureByteStride = sizeof(UINT);

	std::vector<UINT> indexes(maxNumberOfParticles);
	for (int i = 0; i < maxNumberOfParticles; i++)
	{
		indexes[i] = maxNumberOfParticles - 1 - i;
	}

	D3D11_SUBRESOURCE_DATA dlBData = {};
	dlBData.pSysMem = indexes.data();

	Device->CreateBuffer(&dlBDesc, &dlBData, &deadListBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC dlUAVDesc = {};
	dlUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	dlUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	dlUAVDesc.Buffer.NumElements = maxNumberOfParticles;
	dlUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;

	Device->CreateUnorderedAccessView(deadListBuffer, &dlUAVDesc, &deadListUAV);
	Context->CSSetUnorderedAccessViews(1, 1, &deadListUAV, &maxNumberOfParticles);

	D3D11_BUFFER_DESC slBDesc = {};
	slBDesc.Usage = D3D11_USAGE_DEFAULT;
	slBDesc.ByteWidth = sizeof(SortingData) * maxNumberOfParticles;
	slBDesc.CPUAccessFlags = 0;
	slBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	slBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	slBDesc.StructureByteStride = sizeof(SortingData);

	std::vector<SortingData> sortingData(maxNumberOfParticles);
	for (unsigned int i = 0; i < maxNumberOfParticles; i++)
	{
		sortingData[i] = { i, 0.0f };
	}
	D3D11_SUBRESOURCE_DATA slBData = {};
	slBData.pSysMem = sortingData.data();

	Device->CreateBuffer(&slBDesc, &slBData, &sortedListBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC slUAVDesc = {};
	slUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	slUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	slUAVDesc.Buffer.NumElements = maxNumberOfParticles;
	slUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;

	Device->CreateUnorderedAccessView(sortedListBuffer, &slUAVDesc, &sortedListUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC slSRVDesc = {};
	slSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	slSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	slSRVDesc.BufferEx.FirstElement = 0;
	slSRVDesc.BufferEx.NumElements = maxNumberOfParticles;

	Device->CreateShaderResourceView(sortedListBuffer, &slSRVDesc, &sortedListSRV);
	
	D3D11_BUFFER_DESC iBDesc = {};
	iBDesc.Usage = D3D11_USAGE_DEFAULT;
	iBDesc.ByteWidth = sizeof(UINT) * 4;
	iBDesc.CPUAccessFlags = 0;
	iBDesc.BindFlags = 0;
	iBDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	iBDesc.StructureByteStride = sizeof(UINT);

	UINT initData[4] = {0, 1, 0, 0};
	D3D11_SUBRESOURCE_DATA iBData = {};
	iBData.pSysMem = initData;

	Device->CreateBuffer(&iBDesc, &iBData, &indirectBuffer);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstBufferData);

	constBufferData.world = Matrix::CreateTranslation(Vector3::Up * 4);
	World = Matrix::CreateTranslation(Vector3::Up * 4);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &constBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&constBufDesc, &constData, &constBuffer);

	D3D11_BUFFER_DESC emitterBufDesc = {};
	emitterBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	emitterBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	emitterBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	emitterBufDesc.MiscFlags = 0;
	emitterBufDesc.StructureByteStride = 0;
	emitterBufDesc.ByteWidth = sizeof(Emitter);

	D3D11_SUBRESOURCE_DATA emitterData = {};
	emitterData.pSysMem = &emitter;
	emitterData.SysMemPitch = 0;
	emitterData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&emitterBufDesc, &emitterData, &emitterBuffer);

	D3D11_BUFFER_DESC sortingBufDesc = {};
	sortingBufDesc.Usage = D3D11_USAGE_DEFAULT;
	sortingBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sortingBufDesc.CPUAccessFlags = 0;
	sortingBufDesc.MiscFlags = 0;
	sortingBufDesc.StructureByteStride = 0;
	sortingBufDesc.ByteWidth = sizeof(SortingCB);

	Device->CreateBuffer(&sortingBufDesc, nullptr, &sortingCBuffer);
}

void ParticleSystem::InitHeightMap(int width)
{
	D3D11_TEXTURE2D_DESC hmDesc = {};
	hmDesc.ArraySize = 1;
	hmDesc.MipLevels = 1;
	hmDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	hmDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	hmDesc.CPUAccessFlags = 0;
	hmDesc.MiscFlags = 0;
	hmDesc.Usage = D3D11_USAGE_DEFAULT;
	hmDesc.Width = width;
	hmDesc.Height = width;
	hmDesc.SampleDesc = { 1, 0 };

	Device->CreateTexture2D(&hmDesc, nullptr, &heightMap);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	Device->CreateShaderResourceView(heightMap, &shaderResourceViewDesc, &HeightView);
	Device->CreateDepthStencilView(heightMap, &depthStencilViewDesc, &DepthView);

	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	Device->CreateSamplerState(&comparisonSamplerDesc, &HMSamplerState);

	viewport = Viewport(0.0f, 0.0f, width, width);

	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"HMVSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&HMVertexShaderByteCode,
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << "../Shaders/ParticleShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		HMVertexShaderByteCode->GetBufferPointer(),
		HMVertexShaderByteCode->GetBufferSize(),
		nullptr, &HMVertexShader);

	///const buffer initialization
	D3D11_BUFFER_DESC hmBufDesc = {};
	hmBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	hmBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hmBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hmBufDesc.MiscFlags = 0;
	hmBufDesc.StructureByteStride = 0;
	hmBufDesc.ByteWidth = sizeof(HeightMap);

	heightMapData.viewProjection = Matrix::Identity;

	D3D11_SUBRESOURCE_DATA hmBufData = {};
	hmBufData.pSysMem = &heightMapData;
	hmBufData.SysMemPitch = 0;
	hmBufData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&hmBufDesc, &hmBufData, &heightMapBuf);
}

void ParticleSystem::LoadTexture()
{
	std::wstring fileName = L"../Textures/Raindrop.png";
	DirectX::ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(fileName.c_str(), L".dds") == 0)
	{
		hr = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".tga") == 0)
	{
		hr = LoadFromTGAFile(fileName.c_str(), nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".hdr") == 0)
	{
		hr = LoadFromHDRFile(fileName.c_str(), nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, image);
	}


	if (SUCCEEDED(hr))
	{
		hr = CreateShaderResourceView(Device.Get(),
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &textureView);
	}
	else
	{
		std::cout << "Missing particle texture\n";
	}

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MaxLOD = INT_MAX;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 16;

	Device->CreateSamplerState(&sampDesc, &samplerState);
}

void ParticleSystem::LoadShaders()
{
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << "../Shaders/ParticleShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorVertexCode);

	Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"GSMain",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&geometryShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
	}

	Device->CreateGeometryShader(
		geometryShaderByteCode->GetBufferPointer(),
		geometryShaderByteCode->GetBufferSize(),
		nullptr, &geometryShader);

	res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"SimulateCS",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&simulateShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
	}

	Device->CreateComputeShader(
		simulateShaderByteCode->GetBufferPointer(),
		simulateShaderByteCode->GetBufferSize(),
		nullptr, &simulateShader);

	res = D3DCompileFromFile(L"../Shaders/ParticleShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"EmitCS",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&emitShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
	}

	Device->CreateComputeShader(
		emitShaderByteCode->GetBufferPointer(),
		emitShaderByteCode->GetBufferSize(),
		nullptr, &emitShader);

	res = D3DCompileFromFile(L"../Shaders/SortingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"BitonicSort",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&sortShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
	}

	Device->CreateComputeShader(
		sortShaderByteCode->GetBufferPointer(),
		sortShaderByteCode->GetBufferSize(),
		nullptr, &sortShader);

	res = D3DCompileFromFile(L"../Shaders/SortingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"MatrixTranspose",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&transposeShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
	}

	Device->CreateComputeShader(
		transposeShaderByteCode->GetBufferPointer(),
		transposeShaderByteCode->GetBufferSize(),
		nullptr, &transposeShader);
}


void ParticleSystem::UpdateConstBuffer() {

	///const buffer update
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	Context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &constBufferData, sizeof(ConstBufferData));

	Context->Unmap(constBuffer, 0);

	Context->Map(emitterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &emitter, sizeof(Emitter));

	Context->Unmap(emitterBuffer, 0);
}

void ParticleSystem::SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight)
{
	SortingCB cb = { iLevel, iLevelMask, iWidth, iHeight };
	Context->UpdateSubresource(sortingCBuffer, 0, nullptr, &cb, 0, 0);
	Context->CSSetConstantBuffers(0, 1, &sortingCBuffer);
}

void ParticleSystem::SortParticles()
{
	const UINT MATRIX_HEIGHT = maxNumberOfParticles / BITONIC_BLOCK_SIZE;

	// Sort the data
	// First sort the rows for the levels <= to the block size
	for (UINT level = 2; level <= BITONIC_BLOCK_SIZE; level = level * 2)
	{
		SetConstants(level, level, MATRIX_HEIGHT, MATRIX_WIDTH);

		// Sort the row data
		Context->CSSetUnorderedAccessViews(0, 1, &sortedListUAV, nullptr);
		Context->CSSetShader(sortShader, nullptr, 0);
		Context->Dispatch(maxNumberOfParticles / BITONIC_BLOCK_SIZE, 1, 1);
	}

	// Then sort the rows and columns for the levels > than the block size
	// Transpose. Sort the Columns. Transpose. Sort the Rows.
	for (UINT level = (BITONIC_BLOCK_SIZE * 2); level <= maxNumberOfParticles; level = level * 2)
	{
		SetConstants((level / BITONIC_BLOCK_SIZE), (level & ~maxNumberOfParticles) / BITONIC_BLOCK_SIZE, MATRIX_WIDTH, MATRIX_HEIGHT);

		// Transpose the data from buffer 1 into buffer 2
		ID3D11ShaderResourceView* pViewnullptr = nullptr;
		Context->CSSetShaderResources(0, 1, &pViewnullptr);
		Context->CSSetUnorderedAccessViews(0, 1, &sortedListUAV, nullptr);
		// Context->CSSetShaderResources(0, 1, &g_pBuffer1SRV);
		Context->CSSetShader(transposeShader, nullptr, 0);
		Context->Dispatch(MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the transposed column data
		Context->CSSetShader(sortShader, nullptr, 0);
		Context->Dispatch(maxNumberOfParticles / BITONIC_BLOCK_SIZE, 1, 1);

		SetConstants(BITONIC_BLOCK_SIZE, level, MATRIX_HEIGHT, MATRIX_WIDTH);

		// Transpose the data from buffer 2 back into buffer 1
		Context->CSSetShaderResources(0, 1, &pViewnullptr);
		Context->CSSetUnorderedAccessViews(0, 1, &sortedListUAV, nullptr);
		//Context->CSSetShaderResources(0, 1, &g_pBuffer2SRV);
		Context->CSSetShader(transposeShader, nullptr, 0);
		Context->Dispatch(MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the row data
		Context->CSSetShader(sortShader, nullptr, 0);
		Context->Dispatch(maxNumberOfParticles / BITONIC_BLOCK_SIZE, 1, 1);
	}
}

UINT ParticleSystem::GetDispatchDim(UINT nParticles)
{
	int numGroups = (nParticles % THREAD_GROUP_TOTAL != 0) ?
		((nParticles / THREAD_GROUP_TOTAL) + 1) :
		(nParticles / THREAD_GROUP_TOTAL);
	double secondRoot = sqrt((double)numGroups);
	return std::ceil(secondRoot);
}
