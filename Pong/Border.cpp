#include "Border.h"

#include "RenderComponent.h"

Border::Border() {
	renderComponent = new RenderComponent("../Shaders/StripesShader.hlsl");
	components.push_back(renderComponent);
}

void Border::Update()
{
}

void Border::Initialize()
{
	DirectX::SimpleMath::Rectangle r(-10, 400, 20, 800);
	renderComponent->Add2DRect(r);

	GameObject::Initialize();
}
