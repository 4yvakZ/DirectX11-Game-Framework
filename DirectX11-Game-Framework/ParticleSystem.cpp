#include "ParticleSystem.h"

#include "DirectXTex.h"

ParticleSystem::~ParticleSystem()
{
	textureView->Release();
	samplerState->Release();
}

void ParticleSystem::Initialize()
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
		hr = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
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
