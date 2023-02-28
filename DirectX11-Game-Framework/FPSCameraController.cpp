#include "FPSCameraController.h"

#include "Game.h"
#include "Camera.h"

FPSCameraController::FPSCameraController(Camera* camera) :
	CameraController(camera) 
{
	Game::GetInputDevice()->MouseMove.AddRaw(this, &FPSCameraController::MouseEventHandler, 10);
}

void FPSCameraController::Update(float deltaTime)
{
	InputDevice* inputDevice = Game::GetInputDevice();

	Vector3 cameraPos = camera->position;

	//TODO: Change to smarter rotation;

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(camera->yaw, camera->pitch, 0);

	Vector3 cameraForward = Vector3::Transform(Vector3::Forward, rotationMatrix);
	Vector3 cameraRight = Vector3::Transform(Vector3::Right, rotationMatrix);

	if (inputDevice->IsKeyDown(Keys::A)) {
		cameraPos += cameraSpeed * deltaTime * cameraRight;
	}
	if (inputDevice->IsKeyDown(Keys::D)) {
		cameraPos -= cameraSpeed * deltaTime * cameraRight;
	}
	if (inputDevice->IsKeyDown(Keys::W)) {
		cameraPos -= cameraSpeed * deltaTime * cameraForward;
	}
	if (inputDevice->IsKeyDown(Keys::S)) {
		cameraPos += cameraSpeed * deltaTime * cameraForward;
	}
	if (inputDevice->IsKeyDown(Keys::Space)) {
		cameraPos += cameraSpeed * deltaTime * Vector3::Up;
	}
	if (inputDevice->IsKeyDown(Keys::LeftShift)) {
		cameraPos -= cameraSpeed * deltaTime * Vector3::Up;
	}

	Vector3 cameraTarget = cameraPos +
		Vector3::Transform(Vector3::Backward, rotationMatrix);

	camera->target = cameraTarget;
	camera->position = cameraPos;
}

void FPSCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	camera->yaw += -mouseData.Offset.x * cameraRotationSpeed;
	camera->pitch += mouseData.Offset.y * cameraRotationSpeed;

	camera->fovAngle += mouseData.WheelDelta * cameraFOVSpeed;
	if (camera->fovAngle > DirectX::XM_PI - DirectX::XM_PIDIV4) {
		camera->fovAngle = DirectX::XM_PI - DirectX::XM_PIDIV4;
	}
	if (camera->fovAngle < DirectX::XM_PIDIV4) {
		camera->fovAngle = DirectX::XM_PIDIV4;
	}
}
