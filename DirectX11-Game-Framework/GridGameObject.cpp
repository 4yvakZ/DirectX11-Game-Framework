#include "GridGameObject.h"

#include "LineRenderComponent.h"

GridGameObject::GridGameObject()
{
	gridRenderComponent = new LineRenderComponent("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(gridRenderComponent);
}

void GridGameObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void GridGameObject::Initialize()
{
	gridRenderComponent->AddGrid(10, 2, Color(1, 1, 1, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(2, 0, 0, 1), Color(1, 0, 0, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(0, 2, 0, 1), Color(0, 1, 0, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(0, 0, 2, 1), Color(0, 0, 1, 1));

	GameObject::Initialize();
}
