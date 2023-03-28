#pragma once
#include "framework.h"

using namespace DirectX::SimpleMath;

class CameraController;
class GameObject;

class GAMEFRAMEWORK_API Camera
{
public:

	Camera();

	void Update(float deltaTime);

	Vector3 GetForwardVector();
	Vector3 GetRightVector();
	Vector3 GetUpVector();

	CameraController* controller;

public:

	bool isPerspectiveProjection = true;

	Matrix view;
	Matrix projection;

	Vector3 position;

	Vector3 target;
	Vector3 up;

	float farPlane = 40.0f;
	float nearPlane = 0.1f;
	float fovAngle = DirectX::XM_PIDIV2;
	float aspectRatio;

	int ortWidth;
	int ortHeight;

	GameObject* targetObject;
};

