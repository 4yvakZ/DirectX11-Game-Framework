#include "FPSCameraController.h"

#include "Game.h"
#include "Camera.h"
#include "GameObject.h"

FPSCameraController::FPSCameraController(Camera* camera) :
	CameraController(camera) 
{
	Game::GetInputDevice()->MouseMove.AddRaw(this, &FPSCameraController::MouseEventHandler, 10);
}

void FPSCameraController::Update(float deltaTime)
{

	Vector3 cameraPos;

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);

	if (camera->targetObject)
	{
		cameraPos = camera->targetObject->GetPosition();
	}
	else
	{
		cameraPos = camera->position;

		Vector3 cameraForward = Vector3::Transform(Vector3::Forward, rotationMatrix);
		Vector3 cameraRight = Vector3::Transform(Vector3::Right, rotationMatrix);

		InputDevice* inputDevice = Game::GetInputDevice();

		if (inputDevice->IsKeyDown(Keys::A))
		{
			cameraPos += cameraSpeed * deltaTime * cameraRight;
		}
		if (inputDevice->IsKeyDown(Keys::D))
		{
			cameraPos -= cameraSpeed * deltaTime * cameraRight;
		}
		if (inputDevice->IsKeyDown(Keys::W))
		{
			cameraPos -= cameraSpeed * deltaTime * cameraForward;
		}
		if (inputDevice->IsKeyDown(Keys::S))
		{
			cameraPos += cameraSpeed * deltaTime * cameraForward;
		}
		if (inputDevice->IsKeyDown(Keys::Space))
		{
			cameraPos += cameraSpeed * deltaTime * Vector3::Up;
		}
		if (inputDevice->IsKeyDown(Keys::LeftShift))
		{
			cameraPos -= cameraSpeed * deltaTime * Vector3::Up;
		}
	}
	
	Vector3 cameraTarget = cameraPos +
		Vector3::Transform(Vector3::Backward, rotationMatrix);
	camera->target = cameraTarget;
	camera->position = cameraPos;
}

void FPSCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	yaw += -mouseData.Offset.x * cameraRotationSpeed;
	pitch += mouseData.Offset.y * cameraRotationSpeed * 2;
	if (pitch > DirectX::XM_PIDIV2 - 0.01)
	{
		pitch = DirectX::XM_PIDIV2 - 0.01;
	}
	if (pitch < -DirectX::XM_PIDIV2 + 0.01)
	{
		pitch = -DirectX::XM_PIDIV2 + 0.01;
	}
	pitch = pitch;

	camera->fovAngle += mouseData.WheelDelta * cameraFOVSpeed;
	if (camera->fovAngle > DirectX::XM_PI - DirectX::XM_PIDIV4) 
	{
		camera->fovAngle = DirectX::XM_PI - DirectX::XM_PIDIV4;
	}
	if (camera->fovAngle < DirectX::XM_PIDIV4) 
	{
		camera->fovAngle = DirectX::XM_PIDIV4;
	}
}

Vector3 FPSCameraController::GetForwardVector()
{
	return Vector3::Transform(Vector3::Forward, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}

Vector3 FPSCameraController::GetRightVector()
{
	return Vector3::Transform(Vector3::Right, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}

Vector3 FPSCameraController::GetUpVector()
{
	return Vector3::Transform(Vector3::Up, Matrix::CreateFromYawPitchRoll(yaw, pitch, 0));
}
