#include "SphereCameraController.h"

#include "Game.h"
#include "InputDevice.h"
#include "Camera.h"

SphereCameraController::SphereCameraController(Camera* camera):
	CameraController(camera)
{
	Game::GetInputDevice()->MouseMove.AddRaw(this, &SphereCameraController::MouseEventHandler, 10);
}

void SphereCameraController::Update(float deltaTime)
{
	InputDevice* inputDevice = Game::GetInputDevice();

	Vector3 cameraPos = camera->position;
	Vector3 cameraTarget = camera->target;
	Vector3 cameraForward = cameraTarget - cameraPos;
	cameraForward.Normalize();

	Matrix translateToZero = Matrix::CreateTranslation(-cameraPos);

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	Matrix translateToArm = Matrix::CreateTranslation(cameraForward * armLength + cameraPos);

	cameraForward = Vector3::Transform(Vector3::Forward, rotationMatrix);

	Matrix translateToNewPos = Matrix::CreateTranslation(-cameraForward * armLength);
	
	cameraPos = Vector3::Transform(cameraPos, translateToZero * rotationMatrix * translateToArm);

	Vector3 cameraForwardProjected = cameraForward;
	cameraForwardProjected.y = 0;
	cameraForwardProjected.Normalize();

	Vector3 cameraRightProjected = Vector3::Transform(Vector3::Right, rotationMatrix);
	cameraRightProjected.y = 0;
	cameraRightProjected.Normalize();
	

	if (inputDevice->IsKeyDown(Keys::A))
	{
		cameraPos -= cameraSpeed * deltaTime * cameraRightProjected;
	}
	if (inputDevice->IsKeyDown(Keys::D))
	{
		cameraPos += cameraSpeed * deltaTime * cameraRightProjected;
	}
	if (inputDevice->IsKeyDown(Keys::W))
	{
		cameraPos += cameraSpeed * deltaTime * cameraForwardProjected;
	}
	if (inputDevice->IsKeyDown(Keys::S))
	{
		cameraPos -= cameraSpeed * deltaTime * cameraForwardProjected;
	}
	if (inputDevice->IsKeyDown(Keys::Space))
	{
		cameraPos += cameraSpeed * deltaTime * Vector3::Up;
	}
	if (inputDevice->IsKeyDown(Keys::LeftShift))
	{
		cameraPos -= cameraSpeed * deltaTime * Vector3::Up;
	}

	cameraPos = Vector3::Transform(cameraPos, translateToNewPos);
	cameraTarget = cameraPos +
		Vector3::Transform(Vector3::Forward, rotationMatrix);

	camera->target = cameraTarget;
	camera->position = cameraPos;
}

void SphereCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	yaw += -mouseData.Offset.x * cameraRotationSpeed;
	pitch += -mouseData.Offset.y * cameraRotationSpeed;
	if (pitch > DirectX::XM_PIDIV2 - 0.01)
	{
		pitch = DirectX::XM_PIDIV2 - 0.01;
	}
	if (pitch < -DirectX::XM_PIDIV2 + 0.01)
	{
		pitch = -DirectX::XM_PIDIV2 + 0.01;
	}
}
