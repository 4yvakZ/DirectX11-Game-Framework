#include "FPSCameraController.h"

#include "Game.h"
#include "InputDevice.h"

void FPSCameraController::Update(float deltaTime)
{
	InputDevice* inputDevice = Game::GetInputDevice();

	Matrix cameraRotation = Matrix::CreateFromYawPitchRoll(camera->yaw, camera->pitch, 0);

	Vector3 cameraPos = camera->position;

	Vector3 cameraForward = Vector3::Transform(Vector3::Forward, cameraRotation);
	Vector3 cameraRight = Vector3::Transform(Vector3::Right, cameraRotation);

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
		Vector3::Transform(Vector3::Backward, cameraRotation);

	camera->target = cameraTarget;
	camera->position = cameraPos;
}
