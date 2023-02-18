#include "TestGameObject.h"

#include "Component.h"
#include "RenderComponent.h"

TestGameObject::TestGameObject()
{
	renderComponent = new RenderComponent("./Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);
	renderComponent->AddPoint(DirectX::XMFLOAT4(0.3f, 0.7f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	renderComponent->AddPoint(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	renderComponent->AddPoint(DirectX::XMFLOAT4(0.5f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	renderComponent->AddIndex(0);
	renderComponent->AddIndex(1);
	renderComponent->AddIndex(2);
}

void TestGameObject::Update()
{
}

void TestGameObject::Initialize()
{
	for (auto component : components) {
		component->Initialize();
	}
}
