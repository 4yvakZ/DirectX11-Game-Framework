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

	Vector3 cameraForward = Vector3::Transform(Vector3::Forward, camera->rotation);
	Vector3 cameraRight = Vector3::Transform(Vector3::Right, camera->rotation);

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
		Vector3::Transform(Vector3::Backward, camera->rotation);

	camera->target = cameraTarget;
	camera->position = cameraPos;
}

void FPSCameraController::MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	Vector3 cameraUp = Vector3::Transform(Vector3::Up, camera->rotation);
	Vector3 cameraRight = Vector3::Transform(Vector3::Right, camera->rotation);

	float deltaYaw = -mouseData.Offset.x * cameraRotationSpeed;
	float deltaPitch = mouseData.Offset.y * cameraRotationSpeed;

	Quaternion rotator = Quaternion::CreateFromYawPitchRoll(deltaYaw, deltaPitch, 0);

	//std::cout << rotator.x << " " << rotator.y << " " << rotator.z << " " << rotator.w << "\n";

	camera->rotation *= Quaternion::CreateFromYawPitchRoll(deltaYaw, deltaPitch, 0);
	//std::cout << camera->rotation.x << " " << camera->rotation.y << " " << camera->rotation.z << " " << camera->rotation.w << "\n\n";


	camera->fovAngle += mouseData.WheelDelta * cameraFOVSpeed;
	if (camera->fovAngle > DirectX::XM_PI - DirectX::XM_PIDIV4) {
		camera->fovAngle = DirectX::XM_PI - DirectX::XM_PIDIV4;
	}
	if (camera->fovAngle < DirectX::XM_PIDIV4) {
		camera->fovAngle = DirectX::XM_PIDIV4;
	}
}
