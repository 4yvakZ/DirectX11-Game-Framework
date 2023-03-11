#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent2D.h"
#include "RenderComponentFBX.h"

TestGameObject::TestGameObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/guitar/guitar.fbx", "../FBX/guitar/TD_Checker.png");
	SetScale(4);
	//FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/cube.fbx");
	components.push_back(FbxRenderComponent);
	renderComponent = new RenderComponent2D("../Shaders/SimpleTextureShader.hlsl");
	components.push_back(renderComponent);
}

void TestGameObject::Update(float deltaTime)
{
	SetRotation(GetRotation() 
		//* Quaternion::CreateFromAxisAngle(Vector3::Right, speed * deltaTime)
		* Quaternion::CreateFromAxisAngle(Vector3::Up, speed * deltaTime));
    FbxRenderComponent->World = GetWorld();
	renderComponent->constBufferData.worldViewPosition *= Matrix::CreateTranslation(Vector3(0.001, 0.001, 0));
	//std::cout << renderComponent->offset.x << std::endl;

	GameObject::Update(deltaTime);
}

void TestGameObject::Initialize()
{
	renderComponent->AddTriangle(Vector3(0.25f, 0.7f, 0.5f), Vector3(0.8f, 0.3f, 0.5f), Vector3(0.5f, 1.0f, 0.5f));

	//DirectX::SimpleMath::Rectangle r(-100, 400, 200, 800);
	//renderComponent->Add2DRect(r, Color(1, 0, 0, 1));

	//for (int i = 64; i > 2; i /= 2) {
	//	renderComponent->Add2DCircle(Vector4(), 100, i, Color(rand() % 10 / 10.f, rand() % 10 / 10.f, rand() % 10 / 10.f));
	//}
	//renderComponent->Add2DCircle(Vector3(-300, -300, 0), 20);

	GameObject::Initialize();
}
