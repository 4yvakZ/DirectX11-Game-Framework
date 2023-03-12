#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent2D.h"
#include "RenderComponentFBX.h"

TestGameObject::TestGameObject()
{
	//FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/guitar/guitar.fbx", "../FBX/guitar/TD_Checker.png");
	//FbxRenderComponent = new RenderComponentFBX("../FBX/Winter_Mug/Winter_Mug.fbx", "../FBX/Winter_Mug/Winter_Mug.png");
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/pill_bottle/pill_bottle.fbx", "../FBX/pill_bottle/TD_Checker.png");
	SetScale(4);
	//FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/cube.fbx");
	components.push_back(FbxRenderComponent);
	renderComponent = new RenderComponent2D("../Shaders/SimpleTextureShader.hlsl");
	components.push_back(renderComponent);
}

void TestGameObject::Update(float deltaTime)
{
	SetRotation(GetRotation() * Quaternion::CreateFromAxisAngle(Vector3::Up, speed * deltaTime));
	SetPosition(GetPosition() + speed * Vector3(0.05, 0.05, 0) * deltaTime);
    FbxRenderComponent->World = GetWorld();
	renderComponent->constBufferData.worldViewPosition *= Matrix::CreateTranslation(Vector3(0.001, 0.001, 0));
	//std::cout << renderComponent->offset.x << std::endl;

	GameObject::Update(deltaTime);
}

void TestGameObject::Initialize()
{
	renderComponent->AddTriangle(Vector3(0.25f, 0.7f, 0.5f), Vector3(0.8f, 0.3f, 0.5f), Vector3(0.5f, 1.0f, 0.5f));

	GameObject::Initialize();
}
