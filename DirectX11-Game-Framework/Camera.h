#pragma once
#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API Camera
{
public:
	Camera();

	void Update();

	Matrix GetWorldViewPositionMatrix(Matrix World);

	Matrix view;
	Matrix projection;

	Vector3 position;
	Vector3 target;
	Vector3 up;

	bool isPerspectiveProjection = true;
};

