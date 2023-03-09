#pragma once
#include "framework.h"

using namespace DirectX::SimpleMath;

struct GAMEFRAMEWORK_API Transform {

	Vector3 position;
	Quaternion rotation;
	Vector3 scale{ 1.0f, 1.0f, 1.0f };
	
	Matrix world;
};