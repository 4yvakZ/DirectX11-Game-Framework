#include "Racket.h"

#include "RenderComponent.h"
#include "Game.h"
#include "InputDevice.h"
#include "Ball.h"

Racket::Racket()
{
	renderComponent = new RenderComponent("../Shaders/SimpleShader.hlsl");
	components.push_back(renderComponent);
}

void Racket::Update(float deltaTime)
{

	if (Game::GetInputDevice()->IsKeyDown(upKey)) {
		renderComponent->offset += Vector4(0, speed, 0, 0) * deltaTime;
	} 
	if (Game::GetInputDevice()->IsKeyDown(downKey)) {
		renderComponent->offset -= Vector4(0, speed, 0, 0) * deltaTime;
	}

	rect.y = renderComponent->offset.y * 400 + rect.height / 2;

	if (rect.y > 400) {
		renderComponent->offset.y = (400 - rect.height / 2) / 400.0f;
		rect.y = 400;
	}

	if (rect.y  -  rect.height < -400) {
		renderComponent->offset.y = (-400 + rect.height / 2) / 400.0f;
		rect.y = -400 + rect.height;
	}

	if (ball->x + ball->radius >= rect.x && 
		ball->x - ball->radius <= rect.x + rect.width 
		&& ball->y <= rect.y && 
		ball->y >= rect.y - rect.height) {
		float rectCenterY = rect.y - rect.height / 2;
		ball->Bounce((ball->y - rectCenterY) / (rect.y + 20 - rectCenterY));
	}


	GameObject::Update(deltaTime);
}

void Racket::Initialize()
{
	renderComponent->Add2DRect(rect, Color(1, 0, 0, 1));

	GameObject::Initialize();
}
