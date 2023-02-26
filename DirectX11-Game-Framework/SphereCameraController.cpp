#include "SphereCameraController.h"

#include "Game.h"
#include "InputDevice.h"

SphereCameraController::SphereCameraController(Camera* camera):
	CameraController(camera)
{
	Game::GetInputDevice()->MouseMove.AddRaw(this, &SphereCameraController::MouseEventHandler, 10);
}

void SphereCameraController::Update(float deltaTime)
{
}

void SphereCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
}
