#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API  DirectionalLight
{
	struct LightData
	{
		Vector4 direction;
		Vector4 intensity{ 1.5f, 1.5f, 1.5f, 0.0f };
	};
};

