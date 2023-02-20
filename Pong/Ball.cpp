#include "Ball.h"
#include "RenderComponent.h"

Ball::Ball()
{
	renderComponent = new RenderComponent("../Shaders/MyVeryFirstShader.hlsl");
	components.push_back(renderComponent);
}

void Ball::Update()
{
}

void Ball::Initialize()
{
	renderComponent->Add2DCircle(Vector4(-300, -300, 0, 1), 15);

	GameObject::Initialize();
}
