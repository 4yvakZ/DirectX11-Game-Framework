#include "Ball.h"
#include "RenderComponent.h"

Ball::Ball()
{
	renderComponent = new RenderComponent("../Shaders/SimpleShader.hlsl");
	components.push_back(renderComponent);
}

void Ball::Update(float deltaTime)
{

	renderComponent->offset += Vector4(xv, yv, 0, 0) * deltaTime;
	x = renderComponent->offset.x * 600;
	y = renderComponent->offset.y * 400;

	if (y + radius > 400 && yv > 0) {
		yv *= -1;
	}
	if (y - radius < -400 && yv < 0) {
		yv *= -1;
	}

	if (x + radius > 600) {
		counter.Add1();
		renderComponent->offset = Vector4(0, 0, 0, 0);
		speed = 1.5;
		xv = -0.7;
		yv = 0.7;
	}
	if (x - radius < -600) {
		counter.Add2();
		renderComponent->offset = Vector4(0, 0, 0, 0);
		speed = 1.5;
		xv = 0.7;
		yv = 0.7;
	}

	GameObject::Update(deltaTime);
}

void Ball::Initialize()
{
	renderComponent->Add2DCircle(Vector4(x, y, 0, 1), radius);

	GameObject::Initialize();
}

void Ball::Bounce(float yVelocityDirection)
{
	if (x > 0 && xv < 0 || x < 0 && xv > 0) {
		return;
	}
	speed += 0.05;

	yv = speed * sin(yVelocityDirection * DirectX::XM_PI / 2);

	if (xv > 0) {
		xv = -speed * cos(yVelocityDirection * DirectX::XM_PI / 2);
	}
	else {
		xv = speed * cos(yVelocityDirection * DirectX::XM_PI / 2);
	}
	//std::cout << yVelocityDirection << " " << xv << " " << yv << std::endl;
}
