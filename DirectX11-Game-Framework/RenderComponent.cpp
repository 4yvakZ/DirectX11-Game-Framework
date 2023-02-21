#include "RenderComponent.h"

#include "Game.h"
#include "RenderSystem.h"

RenderComponent::~RenderComponent()
{
	RenderSystem* render = Game::GetRenderSystem();
	render->RemoveRenderComponent(this);

	delete layout;
	delete pixelShader;
	delete pixelShaderByteCode;
	delete vertexShader;
	delete vertexShaderByteCode;
}

int RenderComponent::AddRawPoint(Vector4 coords, Color color)
{
	points.push_back(coords);
	points.push_back(color);
	return points.size() / 2 - 1;
}

void RenderComponent::AddIndex(int index)
{
	indexes.push_back(index);
}

void RenderComponent::AddTriangle(Vector4 coords0, Vector4 coords1, Vector4 coords2, Color color)
{
	points.push_back(coords0);
	points.push_back(color);
	int firstPointIndex = points.size() / 2 - 1;
	points.push_back(coords1);
	points.push_back(color);
	points.push_back(coords2);
	points.push_back(color);

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 1);
	indexes.push_back(firstPointIndex + 2);
}

void RenderComponent::Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color)
{
	RenderSystem* render = Game::GetRenderSystem();
	float width = render->viewport.width / 2;
	float height = render->viewport.height / 2;
	float x0 = rect.x / width;
	float x1 = x0 + rect.width / width;
	float y0 = rect.y / height;
	float y1 = y0 - rect.height / height;
	points.push_back(Vector4(x0, y0, 0, 1));
	points.push_back(color);
	int firstPointIndex = points.size() / 2 - 1;
	points.push_back(Vector4(x1, y0, 0, 1));
	points.push_back(color);
	points.push_back(Vector4(x1, y1, 0, 1));
	points.push_back(color);
	points.push_back(Vector4(x0, y1, 0, 1));
	points.push_back(color);

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 1);
	indexes.push_back(firstPointIndex + 2);

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 2);
	indexes.push_back(firstPointIndex + 3);
}

void RenderComponent::Add2DCircle(Vector4 centerCoord, float radius, int numberOfTriangles, Color color)
{
	RenderSystem* render = Game::GetRenderSystem();
	float width = render->viewport.width / 2;
	float height = render->viewport.height / 2;
	float x0 = centerCoord.x / width;
	float y0 = centerCoord.y / height;
	float rx = radius / width;
	float ry = radius / height;


	points.push_back(Vector4(x0, y0, 0, 1));
	points.push_back(color);
	int centerPointIndex = points.size() / 2 - 1;

	double angleStep = DirectX::XM_2PI / numberOfTriangles;

	for (int i = 0; i < numberOfTriangles; i++) {
		points.push_back(Vector4(x0 + rx * cos(i*angleStep),
			y0 + ry * sin(i * angleStep),
			0, 1));
		points.push_back(color);
	}
	
	for (int i = 0; i < numberOfTriangles; i++) {
		indexes.push_back(centerPointIndex);
		if (i == numberOfTriangles - 1) {
			indexes.push_back(centerPointIndex + 1);
		}
		else {
			indexes.push_back(centerPointIndex + 2 + i);
		}
		indexes.push_back(centerPointIndex + 1 + i);
	}
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
			std::cout << "Missing Shader File: " << "MyVeryFirstShader.hlsl" << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

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

	render->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

	//moment for points initialization

	///vertix buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = &points[0];
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
	indexData.pSysMem = &indexes[0];
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
	constBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &offset;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&constBufDesc, &constData, &constBuffer);

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;


	res = render->Device->CreateRasterizerState(&rastDesc, &rastState);
}

void RenderComponent::Update() {
	RenderSystem* render = Game::GetRenderSystem();

	///const buffer update
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	render->Context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &offset, sizeof(offset));

	render->Context->Unmap(constBuffer, 0);
	
}

void RenderComponent::Draw()
{
	RenderSystem* render = Game::GetRenderSystem();
	///Setup Rasterizer stage
	render->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	render->Context->IASetInputLayout(layout);
	render->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	render->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	

	///Set Vertex and Pixel Shaders
	render->Context->VSSetShader(vertexShader, nullptr, 0);
	render->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetShader(pixelShader, nullptr, 0);

	render->Context->DrawIndexed(indexes.size(), 0, 0);
}
