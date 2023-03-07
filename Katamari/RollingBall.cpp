#include "RollingBall.h"

#include "RenderComponentFBX.h"
#include "Game.h"
#include "InputDevice.h"
#include "Camera.h"

RollingBall::RollingBall(float radius, GameObject* parent) :
	radius(radius),
	GameObject(parent)
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/tennis_Ball/tennis_Ball.fbx", "../FBX/tennis_Ball/tennis ball_DefaultMaterial_BaseColor.png");
	FbxRenderComponent->offset = Vector3(0, -radius, 0);
	components.push_back(FbxRenderComponent);
	SetPosition(Vector3(0, radius, 0));
}

RollingBall::~RollingBall()
{
}

void RollingBall::Update(float deltaTime)
{
	Camera* camera = Game::GetCamera();
	Vector3 deltaPos(0, 0, 0);

	Vector3 forward = camera->GetForwardVector();
	forward.y = 0;
	forward.Normalize();

	Vector3 right = camera->GetRightVector();
	right.y = 0;
	right.Normalize();

	InputDevice* inputDevice = Game::GetInputDevice();
	if (inputDevice->IsKeyDown(Keys::A))
	{
		deltaPos += rotationSpeed * radius * deltaTime * right;
	}
	if (inputDevice->IsKeyDown(Keys::D))
	{
		deltaPos -= rotationSpeed * radius * deltaTime * right;
	}
	if (inputDevice->IsKeyDown(Keys::W))
	{
		deltaPos += rotationSpeed * radius * deltaTime * forward;
	}
	if (inputDevice->IsKeyDown(Keys::S))
	{
		deltaPos -= rotationSpeed * radius * deltaTime * forward;
	}

	if (deltaPos != Vector3(0, 0, 0))
	{
		Vector3 rotationAxis = -deltaPos.Cross(Vector3(0, 1, 0));
		rotationAxis.Normalize();
		Quaternion rotator;
		if (rotationAxis.x != 1 && rotationAxis.z != 1)
		{
			 rotator = Quaternion::CreateFromAxisAngle(rotationAxis, deltaTime * rotationSpeed * sqrt(2));
		}
		else
		{
			rotator = Quaternion::CreateFromAxisAngle(rotationAxis, deltaTime * rotationSpeed);
		}

		SetRotation(GetRotation() * rotator);
	}
	
	SetPosition(GetPosition() + deltaPos);
	FbxRenderComponent->World = GetWorld();

	GameObject::Update(deltaTime);
}

void RollingBall::Initialize()
{
	Camera* camera = Game::GetCamera();
	camera->targetObject = this;
	GameObject::Initialize();
}
