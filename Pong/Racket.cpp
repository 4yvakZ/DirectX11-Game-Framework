#include "Racket.h"

#include "RenderComponent.h"

Racket::Racket()
{
	renderComponent = new RenderComponent("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);
}

void Racket::Update()
{
}

void Racket::Initialize()
{
	DirectX::SimpleMath::Rectangle r(-580, 90, 20, 180);
	renderComponent->Add2DRect(r, Color(1, 0, 0, 1));

	GameObject::Initialize();
}
