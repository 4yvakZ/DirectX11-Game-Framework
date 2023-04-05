#include "ShadowMap.h"
#include "Game.h"
#include "Camera.h"


ShadowMap::ShadowMap(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context, int width, Vector4 lightDir):
	lightDir(lightDir)
{
	///const buffer initialization
	D3D11_BUFFER_DESC cascadeBufDesc = {};
	cascadeBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	cascadeBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cascadeBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cascadeBufDesc.MiscFlags = 0;
	cascadeBufDesc.StructureByteStride = 0;
	cascadeBufDesc.ByteWidth = sizeof(CascadeData);

	D3D11_SUBRESOURCE_DATA cascadeBufData = {};
	cascadeBufData.pSysMem = &cascadeData;
	cascadeBufData.SysMemPitch = 0;
	cascadeBufData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&cascadeBufDesc, &cascadeBufData, &cascadeBuffer);

	CreateShaderResources(width, Device);
}

void ShadowMap::CreateShaderResources(int width, Microsoft::WRL::ComPtr<ID3D11Device>& Device)
{
	D3D11_TEXTURE2D_DESC shadowMapDesc = {};
	shadowMapDesc.ArraySize = 4;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.Width = width;
	shadowMapDesc.Height = width;
	shadowMapDesc.SampleDesc = { 1, 0 };

	Device->CreateTexture2D(&shadowMapDesc, nullptr, &shadowMap);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	shaderResourceViewDesc.Texture2DArray.ArraySize = 4;
	

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize = 4;

	Device->CreateShaderResourceView(shadowMap, &shaderResourceViewDesc, &ShadowView);
	Device->CreateDepthStencilView(shadowMap, &depthStencilViewDesc, &DepthView);

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

	Device->CreateSamplerState(&comparisonSamplerDesc, &samplerState);

	viewport = Viewport(0.0f, 0.0f, width, width);

	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/SimpleShadowMapShader.hlsl",
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
			std::cout << "Missing Shader File: " << "../Shaders/SimpleShadowMapShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	res = D3DCompileFromFile(L"../Shaders/SimpleShadowMapShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"GSMain",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&geometryShaderByteCode,
		&errorVertexCode);

	Device->CreateGeometryShader(
		geometryShaderByteCode->GetBufferPointer(),
		geometryShaderByteCode->GetBufferSize(),
		nullptr, &geometryShader);


	///rastState initialization
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.DepthBias = 0.05;
	rastDesc.DepthBiasClamp = 0.1;
	rastDesc.SlopeScaledDepthBias = 4;

	Device->CreateRasterizerState(&rastDesc, &rastState);
}

std::vector<Vector4> ShadowMap::GetFrustrumCornersWorldSpace(const Matrix& view, const Matrix& projection)
{
	const auto inv = (view * projection).Invert();

	std::vector<Vector4> frustrumCorners;
	frustrumCorners.reserve(8);
	for (size_t x = 0; x < 2; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			for (size_t z = 0; z < 2; z++)
			{
				const Vector4 pt = Vector4::Transform(
					Vector4(2.0f * x -1.0f,
						2.0f * y - 1.0f, 
						z, 
						1.0f), inv);
				frustrumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustrumCorners;
}

ShadowMap::~ShadowMap()
{
	DepthView->Release();

	shadowMap->Release();
	ShadowView->Release();

	samplerState->Release();

	vertexShaderByteCode->Release();
	vertexShader->Release();

	geometryShaderByteCode->Release();
	geometryShader->Release();

	rastState->Release();

	cascadeBuffer->Release();
}

void ShadowMap::PrepareFrame(ID3D11DeviceContext* Context) {
	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMap::Render(ID3D11DeviceContext* Context)
{
	Camera* camera = Game::GetCamera();

	constexpr int numOfCascades = 4;
	constexpr float fractions[] = { 0, 0.1, 0.2, 0.4, 1.0 };
	for (size_t i = 0; i < numOfCascades; i++)
	{
		Matrix projection;
		float farPlane = camera->farPlane;
		float nearPlane = camera->nearPlane; 
		cascadeData.distances[i] = (farPlane - nearPlane) * fractions[i + 1] + nearPlane;
		nearPlane = nearPlane + (farPlane - nearPlane) * fractions[i];
		if (camera->isPerspectiveProjection)
		{
			projection = Matrix::CreatePerspectiveFieldOfView(
				camera->fovAngle,
				camera->aspectRatio,
				nearPlane,
				cascadeData.distances[i]);
		}
		else
		{
			projection = Matrix::CreateOrthographic(
				camera->ortWidth,
				camera->ortHeight,
				nearPlane,
				cascadeData.distances[i]);
		}
		auto corners = GetFrustrumCornersWorldSpace(camera->view, projection);

		Vector3 center = Vector3::Zero;
		for (const auto& v : corners)
		{
			center += Vector3(v.x, v.y, v.z);
		}
		center /= corners.size();
		//std::cout << center.x << " " << center.y << " " << center.z << "\n";

		auto view = Matrix::CreateLookAt(center, center + lightDir, Vector3::Up);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (const auto& v : corners)
		{
			auto trf = Vector4::Transform(v, view);
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		constexpr float zMult = 10.0f;

		/*std::cout << minX << " " << maxX << " "
			<< minY << " " << maxY << " "
			<< minZ << " " << maxZ << "\n";*/
		//std::cout << minZ << " " << maxZ << "\n";
		minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
		maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;
		cascadeData.viewProjection[i] = view * Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);
	}

	
	///const buffer update
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto res = Context->Map(cascadeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &cascadeData, sizeof(CascadeData));

	Context->Unmap(cascadeBuffer, 0);


	Context->PSSetConstantBuffers(3, 1, &cascadeBuffer);
	Context->GSSetConstantBuffers(3, 1, &cascadeBuffer);

	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	Context->PSSetShaderResources(1, 1, nullsrv);

	ID3D11RenderTargetView* nullrtv[] = { nullptr,nullptr,nullptr,nullptr, nullptr,nullptr,nullptr,nullptr };
	Context->OMSetRenderTargets(8, nullrtv, DepthView);
	
	Context->RSSetState(rastState);

	Context->RSSetViewports(1, viewport.Get11());
	Context->VSSetShader(vertexShader, nullptr, 0);
	Context->PSSetShader(nullptr, nullptr, 0);
	Context->GSSetShader(geometryShader, nullptr, 0);
}

void ShadowMap::Bind(ID3D11DeviceContext* Context)
{
	Context->PSSetShaderResources(1, 1, &ShadowView);
	Context->PSSetSamplers(1, 1, &samplerState);
	Context->PSSetConstantBuffers(3, 1, &cascadeBuffer);
	Context->GSSetConstantBuffers(3, 1, &cascadeBuffer);
}
