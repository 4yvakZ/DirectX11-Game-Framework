#pragma once
#include "framework.h"
#include "InputDevice.h"

class Camera;

class GAMEFRAMEWORK_API CameraController
{
public:
	CameraController(Camera* camera);

	CameraController() = delete;

	virtual void Update(float deltaTime) = 0;

protected:
	virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload) = 0;

	Camera* camera;
};

