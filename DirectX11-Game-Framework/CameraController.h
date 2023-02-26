#pragma once
#include "framework.h"
#include "Camera.H"

class GAMEFRAMEWORK_API CameraController
{
public:
	CameraController(Camera* camera) :
		camera(camera) {}

	CameraController() = delete;

	virtual void Update(float deltaTime) = 0;

protected:
	Camera* camera;
};

