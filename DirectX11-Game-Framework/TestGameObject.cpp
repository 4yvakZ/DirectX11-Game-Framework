#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent.h"

TestGameObject::TestGameObject()
{
	renderComponent = new RenderComponent("./Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);


	
}

void TestGameObject::Update()
{
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

	DirectX::SimpleMath::Rectangle r(-200, 200, 300, 400);
	renderComponent->Add2DRect(r, Color(1, 0, 0, 1));

	for (int i = 64; i > 2; i /= 2) {
		renderComponent->Add2DCircle(Vector4(), 400, i, Color(1.f / i, 1.f / i, 1.f / i));
	}

	for (auto component : components) {
		component->Initialize();
	}
}
