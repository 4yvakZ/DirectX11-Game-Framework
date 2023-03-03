#pragma once
#include "framework.h"

using namespace DirectX::SimpleMath;

class CameraController;

class GAMEFRAMEWORK_API Camera
{
public:

	Camera();

	void Update(float deltaTime);

	Matrix GetWorldViewPositionMatrix(Matrix World);

	CameraController* controller;

public:

	bool isPerspectiveProjection = true;

	Matrix view;
	Matrix projection;

	Vector3 position;

	Vector3 target;
	Vector3 up;

	float fovAngle = DirectX::XM_PIDIV2;
};

