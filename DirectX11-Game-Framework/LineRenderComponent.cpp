#include "LineRenderComponent.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Camera.h"

inline void LineRenderComponent::AddIndex(int index)
{
    indexes.push_back(index);
}

void LineRenderComponent::AddLine(Vector4 coords0, Vector4 coords1, Color color)
{
	points.push_back(coords0);
	points.push_back(color);
	int firstPointIndex = points.size() / 2 - 1;
	points.push_back(coords1);
	points.push_back(color);

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 1);
}

void LineRenderComponent::AddGrid(int gridSize, float cellSize, Color color)
{

	int firstPointIndex = points.size() / 2;
	int nPoints = gridSize * 2 + 1;
	float offset = -(nPoints / 2) * cellSize;
	for (int i = 0; i < nPoints; i++) {
		for (int j = 0; j < nPoints; j++) {
			points.push_back(Vector4(cellSize * i + offset, 0, cellSize * j + offset, 1));
			points.push_back(color);
			if (j < nPoints - 1) {
				indexes.push_back(firstPointIndex + i * nPoints + j);
				indexes.push_back(firstPointIndex + i * nPoints + j + 1);
			}
			if (i < nPoints - 1) {
				indexes.push_back(firstPointIndex + i * nPoints + j);
				indexes.push_back(firstPointIndex + i * nPoints + j + nPoints);
			}
		}
	}

	std::cout << "\nVertices:\n";
	int i = 0;
	for (const auto& point : points) {
		i++;
		if (i % 2 == 0) continue;
		std::cout << "\n" << i / 2 << ": " << point.x << " " << point.y << " " << point.z << " " << point.w << "\n";
	}

	std::cout << "\nIndexes:";

	i = 0;
	for (const auto& index : indexes) {
		if (i % 2 == 0) std::cout << "\n";

		std::cout << index << " ";
		i++;
	}
	std::cout << "\n";
}

void LineRenderComponent::Draw()
{
	RenderSystem* render = Game::GetRenderSystem();
	///Setup Rasterizer stage
	render->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	render->Context->IASetInputLayout(layout);
	render->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	render->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	render->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);


	///Set Vertex and Pixel Shaders
	render->Context->VSSetShader(vertexShader, nullptr, 0);
	render->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetShader(pixelShader, nullptr, 0);

	render->Context->DrawIndexed(indexes.size(), 0, 0);
}

void LineRenderComponent::Update()
{
	constBufferData.worldViewPosition = Game::GetCamera()->GetWorldViewPositionMatrix(World);
	UpdateConstBuffer();
}
