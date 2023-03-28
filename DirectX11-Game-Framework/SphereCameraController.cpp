#include "SphereCameraController.h"

#include "Game.h"
#include "InputDevice.h"
#include "Camera.h"
#include "GameObject.h"

SphereCameraController::SphereCameraController(Camera* camera):
	CameraController(camera)
{
	Game::GetInputDevice()->MouseMove.AddRaw(this, &SphereCameraController::MouseEventHandler, 10);
}

void SphereCameraController::Update(float deltaTime)
{
	

	Vector3 cameraPos = camera->position;
	Vector3 cameraTarget = camera->target;

	Matrix translateToZero = Matrix::CreateTranslation(-cameraPos);

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	Vector3 cameraForward = -Vector3::Transform(Vector3::Forward, rotationMatrix);

	
	if (camera->targetObject)
	{
		armPosition = camera->targetObject->GetPosition();
	}
	else
	{
		Vector3 cameraForwardProjected = cameraForward;
		cameraForwardProjected.y = 0;
		cameraForwardProjected.Normalize();

		Vector3 cameraRightProjected = Vector3::Transform(Vector3::Right, rotationMatrix);
		cameraRightProjected.y = 0;
		cameraRightProjected.Normalize();

		InputDevice* inputDevice = Game::GetInputDevice();
		if (inputDevice->IsKeyDown(Keys::A))
		{
			armPosition -= cameraSpeed * deltaTime * cameraRightProjected;
		}
		if (inputDevice->IsKeyDown(Keys::D))
		{
			armPosition += cameraSpeed * deltaTime * cameraRightProjected;
		}
		if (inputDevice->IsKeyDown(Keys::W))
		{
			armPosition += cameraSpeed * deltaTime * cameraForwardProjected;
		}
		if (inputDevice->IsKeyDown(Keys::S))
		{
			armPosition -= cameraSpeed * deltaTime * cameraForwardProjected;
		}
		if (inputDevice->IsKeyDown(Keys::Space))
		{
			armPosition += cameraSpeed * deltaTime * Vector3::Up;
		}
		if (inputDevice->IsKeyDown(Keys::LeftShift))
		{
			armPosition -= cameraSpeed * deltaTime * Vector3::Up;
		}
	}

	

	Matrix translateToNewPos = Matrix::CreateTranslation(armPosition - cameraForward * armLength);

	cameraPos = Vector3::Transform(cameraPos, translateToZero * rotationMatrix * translateToNewPos);
	cameraTarget = cameraPos -
		Vector3::Transform(Vector3::Forward, rotationMatrix);

	camera->target = cameraTarget;
	camera->position = cameraPos;
}

void SphereCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	yaw += mouseData.Offset.x * cameraRotationSpeed;
	pitch += mouseData.Offset.y * cameraRotationSpeed;
	if (pitch > DirectX::XM_PIDIV2 - 0.01)
	{
		pitch = DirectX::XM_PIDIV2 - 0.01;
	}
	if (pitch < -DirectX::XM_PIDIV2 + 0.01)
	{
		pitch = -DirectX::XM_PIDIV2 + 0.01;
	}
}

Vector3 SphereCameraController::GetForwardVector()
{
	return -Vector3::Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}

Vector3 SphereCameraController::GetRightVector()
{
	return Vector3::Transform(Vector3::Right, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}

Vector3 SphereCameraController::GetUpVector()
{
	return Vector3::Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}
