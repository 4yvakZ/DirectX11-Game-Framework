#include "PlaneObject.h"

#include "RenderComponentFBX.h"

PlaneObject::PlaneObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/plane.fbx", "../FBX/PNG/Dark/texture_07.png");
	FbxRenderComponent->scale = 5;
	components.push_back(FbxRenderComponent);
}

void PlaneObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void PlaneObject::Initialize()
{
	GameObject::Initialize();
}
