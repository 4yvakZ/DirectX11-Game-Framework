#include "RenderComponent.h"

#include "Game.h"
#include "RenderSystem.h"

RenderComponent::~RenderComponent()
{
	RenderSystem* render = Game::GetRenderSystem();
	render->RemoveRenderComponent(this);

	rastState->Release();

	materialBuffer->Release();
	constBuffer->Release();
	indexBuffer->Release();
	vertexBuffer->Release();

	vertexShader->Release();
	pixelShader->Release();

	vertexShaderByteCode->Release();
	pixelShaderByteCode->Release();
	
	layout->Release();
}

void RenderComponent::Initialize()
{
	RenderSystem* render = Game::GetRenderSystem();

	render->renderComponents.push_back(this);

	ID3DBlob* errorVertexCode = nullptr;
	std::wstring fileName(shaderFileName.begin(), shaderFileName.end());
	auto res = D3DCompileFromFile(fileName.c_str(),
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
			std::cout << "Missing Shader File: " << shaderFileName << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	
	if (textureView) 
	{
		D3D_SHADER_MACRO Shader_Macros[] = {nullptr, nullptr };

		ID3DBlob* errorPixelCode;
		res = D3DCompileFromFile(fileName.c_str(),
			Shader_Macros /*macros*/,
			nullptr /*include*/,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&pixelShaderByteCode,
			&errorPixelCode);

		///samplerState initialization
		D3D11_SAMPLER_DESC sampDesc;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MaxLOD = INT_MAX;
		sampDesc.MipLODBias = 0;
		sampDesc.MaxAnisotropy = 16;

		res = render->Device->CreateSamplerState(&sampDesc, &samplerState);
	}
	else
	{
		D3D_SHADER_MACRO Shader_Macros[] = { "PLAIN", "1",nullptr, nullptr };

		ID3DBlob* errorPixelCode;
		res = D3DCompileFromFile(fileName.c_str(),
			Shader_Macros /*macros*/,
			nullptr /*include*/,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&pixelShaderByteCode,
			&errorPixelCode);
	}

	///vertexShader initialization
	render->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	///pixelShader initialization
	render->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	///layout initialization
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	res = render->Device->CreateInputLayout(
		inputElements,
		std::size(inputElements),
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

	///vertix buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(VertexData) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	///indexBuffer initialization
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indexes);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexes.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	///const buffer initialization
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &constBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&constBufDesc, &constData, &constBuffer);

	///material const buffer initialization
	D3D11_BUFFER_DESC materialBufDesc = {};
	materialBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufDesc.MiscFlags = 0;
	materialBufDesc.StructureByteStride = 0;
	materialBufDesc.ByteWidth = sizeof(MaterialBufferData);

	D3D11_SUBRESOURCE_DATA materialData = {};
	materialData.pSysMem = &materialBufferData;
	materialData.SysMemPitch = 0;
	materialData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&materialBufDesc, &materialData, &materialBuffer);

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;


	res = render->Device->CreateRasterizerState(&rastDesc, &rastState);
}

void RenderComponent::UpdateConstBuffer() {
	RenderSystem* render = Game::GetRenderSystem();

	///const buffer update
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	render->Context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &constBufferData, sizeof(ConstBufferData));

	render->Context->Unmap(constBuffer, 0);
}
