#include "GridGameObject.h"

#include "LineRenderComponent.h"

GridGameObject::GridGameObject()
{
	gridRenderComponent = new LineRenderComponent("../Shaders/SimpleTextureShader.hlsl");
	components.push_back(gridRenderComponent);
}

void GridGameObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void GridGameObject::Initialize()
{
	gridRenderComponent->AddGrid(10, 2, Color(1, 1, 1, 1));

	GameObject::Initialize();
}
