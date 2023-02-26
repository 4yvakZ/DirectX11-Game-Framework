#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent2D.h"
#include "RenderComponentFBX.h"
#include "LineRenderComponent.h"

TestGameObject::TestGameObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/MyVeryFirstShader.hlsl", "../FBX/sphere.fbx");
	components.push_back(FbxRenderComponent);
	renderComponent = new RenderComponent2D("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);
	gridRenderComponent = new LineRenderComponent("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(gridRenderComponent);
}

void TestGameObject::Update(float deltaTime)
{
	renderComponent->constBufferData.offset += Vector4(0.1, 0.1, 0, 0) * deltaTime;
	//std::cout << renderComponent->offset.x << std::endl;

	GameObject::Update(deltaTime);
}

void TestGameObject::Initialize()
{
	///Triangle using points
/*renderComponent->AddPoint(DirectX::XMFLOAT4(0.3f, 0.7f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
renderComponent->AddPoint(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
renderComponent->AddPoint(DirectX::XMFLOAT4(0.5f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
renderComponent->AddIndex(0);
renderComponent->AddIndex(1);
renderComponent->AddIndex(2);*/

	renderComponent->AddTriangle(Vector4(0.25f, 0.7f, 0.5f, 1.0f),
		Vector4(0.5f, 1.0f, 0.5f, 1.0f),
		Vector4(0.8f, 0.3f, 0.5f, 1.0f));

	//DirectX::SimpleMath::Rectangle r(-100, 400, 200, 800);
	//renderComponent->Add2DRect(r, Color(1, 0, 0, 1));

	//for (int i = 64; i > 2; i /= 2) {
	//	renderComponent->Add2DCircle(Vector4(), 100, i, Color(rand() % 10 / 10.f, rand() % 10 / 10.f, rand() % 10 / 10.f));
	//}
	//renderComponent->Add2DCircle(Vector4(-300, -300, 0, 1), 20);

	//gridRenderComponent->AddGrid(5, 1, Color(1, 1, 1, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(1, 0, 0, 1), Color(1, 0, 0, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(0, 1, 0, 1), Color(0, 1, 0, 1));
	gridRenderComponent->AddLine(Vector4(0, 0, 0, 1), Vector4(0, 0, 1, 1), Color(0, 0, 1, 1));

	GameObject::Initialize();
}
