#pragma once
#include "framework.h"
#include "InputDevice.h"

class Camera;

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API CameraController
{
public:
	CameraController(Camera* camera);

	CameraController() = delete;

	virtual void Update(float deltaTime) = 0;

	virtual Vector3 GetForwardVector() = 0;
	virtual Vector3 GetRightVector() = 0;
	virtual Vector3 GetUpVector() = 0;

protected:
	virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload) = 0;

public:
	float yaw = 0;
	float pitch = 0;

protected:
	Camera* camera;

};

