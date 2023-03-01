#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent2D.h"
#include "RenderComponentFBX.h"
#include "LineRenderComponent.h"

TestGameObject::TestGameObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/MyVeryFirstShader.hlsl", "../FBX/cube.fbx");
	components.push_back(FbxRenderComponent);
	renderComponent = new RenderComponent2D("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);
}

void TestGameObject::Update(float deltaTime)
{
	yaw += speed * deltaTime;
	FbxRenderComponent->World = Matrix::CreateFromYawPitchRoll(yaw, 0, 0);
	//renderComponent->constBufferData.offset += Vector4(0.1, 0.1, 0, 0) * deltaTime;
	renderComponent->constBufferData.worldViewPosition *= Matrix::CreateTranslation(Vector3(0.01, 0.01, 0));
	//std::cout << renderComponent->offset.x << std::endl;

	GameObject::Update(deltaTime);
}

void TestGameObject::Initialize()
{
	renderComponent->AddTriangle(Vector4(0.25f, 0.7f, 0.5f, 1.0f), Vector4(0.8f, 0.3f, 0.5f, 1.0f), Vector4(0.5f, 1.0f, 0.5f, 1.0f));

	//DirectX::SimpleMath::Rectangle r(-100, 400, 200, 800);
	//renderComponent->Add2DRect(r, Color(1, 0, 0, 1));

	//for (int i = 64; i > 2; i /= 2) {
	//	renderComponent->Add2DCircle(Vector4(), 100, i, Color(rand() % 10 / 10.f, rand() % 10 / 10.f, rand() % 10 / 10.f));
	//}
	//renderComponent->Add2DCircle(Vector4(-300, -300, 0, 1), 20);

	GameObject::Initialize();
}
