#include "RenderComponentFBX.h"

#include "FBXImporter.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Camera.h"


RenderComponentFBX::RenderComponentFBX(std::string shaderFileName, const char* fileName) :
	fileName(fileName),
	RenderComponent(shaderFileName)
{
	
}

void RenderComponentFBX::Initialize()
{
	FBXImporter importer;
	importer.LoadFBXFile(fileName);
	//std::cout << importer.GetMeshesCount() << "\n";
	
	importer.GetMeshData(0, points, indexes);

	RenderComponent::Initialize();

	std::cout << "\nVertices:\n";
	int i = 0;
	for (const auto& point : points) {
		i++;
		if (i % 2 == 0) continue;
		std::cout << "\n" << i / 2 << ": "
			<< point.x << " "
			<< point.y << " "
			<< point.z << " "
			<< point.w << "\n";
	}

	std::cout << "\nIndexes:\n";
	
	for (const auto& index : indexes) {
		
		std::cout << index << " ";
	}
	std::cout << "\n";

	World = Matrix::Identity;
}

void RenderComponentFBX::Draw()
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

void RenderComponentFBX::Update()
{
	constBufferData.worldViewPosition = Game::GetCamera()->GetWorldViewPositionMatrix(World);
	UpdateConstBuffer();
}
