#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent2D.h"
#include "RenderComponentFBX.h"
#include "LineRenderComponent.h"

TestGameObject::TestGameObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/WinterMug.fbx", "../FBX/WinterMug.png");
	components.push_back(FbxRenderComponent);
	renderComponent = new RenderComponent2D("../Shaders/SimpleTextureShader.hlsl");
	components.push_back(renderComponent);
}

void TestGameObject::Update(float deltaTime)
{
	yaw += speed * deltaTime;
	//FbxRenderComponent->World = Matrix::CreateFromYawPitchRoll(yaw, 0, 0);
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
