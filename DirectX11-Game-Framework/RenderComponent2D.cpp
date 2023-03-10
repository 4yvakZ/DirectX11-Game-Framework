#include "RenderComponent2D.h"

#include "Game.h"
#include "RenderSystem.h"

int RenderComponent2D::AddRawPoint(Vector3 coords, Color color)
{
	points.push_back(VertexData{ coords, color });
	return points.size() - 1;
}

void RenderComponent2D::AddIndex(int index)
{
	indexes.push_back(index);
}

void RenderComponent2D::AddTriangle(Vector3 coords0, Vector3 coords1, Vector3 coords2, Color color)
{

	points.push_back(VertexData{ coords0, color });
	int firstPointIndex = points.size() - 1;
	points.push_back(VertexData{ coords1, color });
	points.push_back(VertexData{ coords2, color });

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 1);
	indexes.push_back(firstPointIndex + 2);
}

void RenderComponent2D::Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color)
{
	RenderSystem* render = Game::GetRenderSystem();
	float width = render->viewport.width / 2;
	float height = render->viewport.height / 2;
	float x0 = rect.x / width;
	float x1 = x0 + rect.width / width;
	float y0 = rect.y / height;
	float y1 = y0 - rect.height / height;

	points.push_back(VertexData{ Vector3{x0, y0, 0}, color});
	int firstPointIndex = points.size() - 1;
	points.push_back(VertexData{ Vector3{x1, y0, 0}, color });
	points.push_back(VertexData{ Vector3{x1, y1, 0}, color });
	points.push_back(VertexData{ Vector3{x0, y1, 0}, color });

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 2);
	indexes.push_back(firstPointIndex + 1);

	indexes.push_back(firstPointIndex);
	indexes.push_back(firstPointIndex + 3);
	indexes.push_back(firstPointIndex + 2);
}

void RenderComponent2D::Add2DCircle(Vector3 centerCoord, float radius, int numberOfTriangles, Color color)
{
	RenderSystem* render = Game::GetRenderSystem();
	float width = render->viewport.width / 2;
	float height = render->viewport.height / 2;
	float x0 = centerCoord.x / width;
	float y0 = centerCoord.y / height;
	float rx = radius / width;
	float ry = radius / height;



	points.push_back(VertexData{ Vector3{x0, y0, 0}, color });
	int centerPointIndex = points.size() - 1;

	double angleStep = DirectX::XM_2PI / numberOfTriangles;

	for (int i = 0; i < numberOfTriangles; i++) {
		points.push_back(VertexData{ 
			Vector3(x0 + rx * cos(i * angleStep),y0 + ry * sin(i * angleStep), 0),
			color });
		
	}
	
	for (int i = 0; i < numberOfTriangles; i++) {
		indexes.push_back(centerPointIndex);
		indexes.push_back(centerPointIndex + 1 + i);
		if (i == numberOfTriangles - 1) {
			indexes.push_back(centerPointIndex + 1);
		}
		else {
			indexes.push_back(centerPointIndex + 2 + i);
		}
	}
}

void RenderComponent2D::Initialize()
{
	
	RenderComponent::Initialize();
}

void RenderComponent2D::Update() {
	UpdateConstBuffer();
}

void RenderComponent2D::Draw()
{
	RenderSystem* render = Game::GetRenderSystem();
	///Setup Rasterizer stage
	render->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };

	render->Context->IASetInputLayout(layout);
	render->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	render->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	

	///Set Vertex and Pixel Shaders
	render->Context->VSSetShader(vertexShader, nullptr, 0);
	render->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetShader(pixelShader, nullptr, 0);
	render->Context->PSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetConstantBuffers(1, 1, &materialBuffer);

	render->Context->DrawIndexed(indexes.size(), 0, 0);
}
