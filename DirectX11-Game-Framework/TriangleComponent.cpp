#include "pch.h"
#include "TriangleComponent.h"

TriangleComponent::TriangleComponent():
	GameComponent()
{
}

void TriangleComponent::DestroyResources()
{
}

void TriangleComponent::Draw()
{
	///Setup Rasterizer stage
	game->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 24 };
	UINT offsets[] = { 0 };

	game->Context->IASetInputLayout(layout);
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, 1, &vertices, strides, offsets);

	///Set Vertex and Pixel Shaders
	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->PSSetShader(pixelShader, nullptr, 0);

	game->Context->DrawIndexed(3, 0, 0);
}

void TriangleComponent::Initialize()
{
	GameComponent::Initialize();
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
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
			std::cout << "Missing Shader File: " << "MyVeryFirstShader.hlsl" << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	///vertexShader initialization
	game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	///pixelShader initialization
	game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	///layout initialization
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
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
			0}
	};

	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

	//moment for points initialization

	///vertices buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertices);

	///indexBuffer initialization
	int indeces[] = { 0,1,2 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;


	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);

	///Setup Rasterizer stage
	game->Context->RSSetState(rastState);
}

void TriangleComponent::Update()
{
}

void TriangleComponent::SetPoints(DirectX::XMFLOAT4 point1, DirectX::XMFLOAT4 point2, DirectX::XMFLOAT4 point3)
{
	points[0] = point1;
	points[2] = point2;
	points[4] = point3;
}

void TriangleComponent::SetColor(DirectX::XMFLOAT4 point1Color, DirectX::XMFLOAT4 point2Color, DirectX::XMFLOAT4 point3Color)
{
	points[1] = point1Color;
	points[3] = point2Color;
	points[5] = point3Color;
}

void TriangleComponent::SetColor(DirectX::XMFLOAT4 Color)
{
	points[1] = Color;
	points[3] = Color;
	points[5] = Color;
}
