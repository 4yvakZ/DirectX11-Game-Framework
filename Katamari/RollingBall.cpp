#include "RollingBall.h"

#include "RenderComponentFBX.h"
#include "Game.h"
#include "InputDevice.h"
#include "Camera.h"
#include "StickyObject.h"

RollingBall::RollingBall(float radius, GameObject* parent) :
	radius(radius),
	targetRadius(radius),
	GameObject(parent)
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", "../FBX/tennis_Ball/tennis_Ball.fbx", "../FBX/tennis_Ball/tennis ball_DefaultMaterial_BaseColor.png");
	FbxRenderComponent->offset = Vector3(0, -0.425, 0);
	FbxRenderComponent->isZUp = false;
	components.push_back(FbxRenderComponent);
	boundingSphere = DirectX::BoundingSphere(Vector3(0, radius, 0), radius * 1.1f);
}

RollingBall::~RollingBall()
{
}

void RollingBall::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
	UpdateCollision();
	GameObject::Update(deltaTime);
}

void RollingBall::Initialize()
{
	Camera* camera = Game::GetCamera();
	camera->targetObject = this;
	GameObject::Initialize();
}

void RollingBall::IncreaseRadius(float additionalRadius)
{
	targetRadius += additionalRadius;
}

void RollingBall::UpdateCollision()
{
	Game* game = Game::GetInstance();
	for (auto& object : game->GameObjects) 
	{
		if (auto stickyObject = dynamic_cast<StickyObject*>(object)) 
		{
			if (stickyObject->radius > radius)
			{
				continue;
			}
			Vector3 centerPoint = stickyObject->GetCenterPoint();
			auto res = boundingSphere.Contains(centerPoint);
			if (res) 
			{
				if (stickyObject->Attach(this)) 
				{
					targetRadius += 0.2;
				}
			}
		}
	}
}

void RollingBall::UpdatePosition(float deltaTime)
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
		deltaPos -= rotationSpeed * radius * deltaTime * right;
	}
	if (inputDevice->IsKeyDown(Keys::D))
	{
		deltaPos += rotationSpeed * radius * deltaTime * right;
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

	if (radius < targetRadius) {
		float radiusAddition = radiusIncreaseSpeed * deltaTime;
		if (radius + radiusAddition > targetRadius) {
			radiusAddition = targetRadius - radius;
		}

		deltaPos += Vector3(0, radiusAddition, 0);
		radius += radiusAddition;
		//std::cout << radius << "\n";
		boundingSphere.Radius = radius * 1.1f;
	}

	SetPosition(GetPosition() + deltaPos);
	boundingSphere.Center = GetPosition();
	FbxRenderComponent->World = GetWorld();
}
