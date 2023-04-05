#include "ParticleSystem.h"

#include "DirectXTex.h"

ParticleSystem::~ParticleSystem()
{
	particlesBuffer->Release();
	particlesUAV->Release();

	deadListBuffer->Release();
	deadListUAV->Release();

	sortedBuffer->Release();
	sortedUAV->Release();

	textureView->Release();
	samplerState->Release();
}

void ParticleSystem::Initialize()
{
	LoadTexture();

	InitResouces();
}

void ParticleSystem::InitResouces()
{
	D3D11_BUFFER_DESC particlesBuffDesc = {};
	particlesBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	particlesBuffDesc.ByteWidth = sizeof(Particle) * maxNumberOfParticles;
	particlesBuffDesc.CPUAccessFlags = 0;
	particlesBuffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	particlesBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	particlesBuffDesc.StructureByteStride = sizeof(Particle);

	Device->CreateBuffer(&particlesBuffDesc, nullptr, &particlesBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC pUAVDesc = {};
	pUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	pUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	pUAVDesc.Buffer.NumElements = maxNumberOfParticles;

	Device->CreateUnorderedAccessView(particlesBuffer, &pUAVDesc, &particlesUAV);

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

	D3D11_BUFFER_DESC sBDesc = {};
	sBDesc.Usage = D3D11_USAGE_DEFAULT;
	sBDesc.ByteWidth = sizeof(SortingData) * maxNumberOfParticles;
	sBDesc.CPUAccessFlags = 0;
	sBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	sBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sBDesc.StructureByteStride = sizeof(SortingData);

	std::vector<SortingData> sortingData(maxNumberOfParticles);
	for (unsigned int i = 0; i < maxNumberOfParticles; i++)
	{
		sortingData[i] = { i, 0.0f };
	}
	D3D11_SUBRESOURCE_DATA sBData = {};
	sBData.pSysMem = sortingData.data();

	Device->CreateBuffer(&sBDesc, &sBData, &sortedBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC sUAVDesc = {};
	sUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	sUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	sUAVDesc.Buffer.NumElements = maxNumberOfParticles;
	sUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;

	Device->CreateUnorderedAccessView(sortedBuffer, &sUAVDesc, &sortedUAV);
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
}
