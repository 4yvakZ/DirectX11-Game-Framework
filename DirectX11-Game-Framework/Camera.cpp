#include "Camera.h"
#include "Game.h"
#include "DisplayWin.h"
#include "CameraController.h"

Camera::Camera()
{
	position = Vector3(0.0f, 2.0f, -5.0f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
}

void Camera::Update(float deltaTime)
{

	if (controller) {
		controller->Update(deltaTime);
	}

	view = Matrix::CreateLookAt(position, target, up);

	int width = Game::GetDisplay()->ClientWidth;
	int height = Game::GetDisplay()->ClientHeight;
	
	if (isPerspectiveProjection) {
		projection = Matrix::CreatePerspectiveFieldOfView(
			fovAngle,
			static_cast<float>(width) / height,
			0.1f,
			1000.0f);
	}
	else {
		projection = Matrix::CreateOrthographic(
			width / 50,
			height / 50,
			0.1,
			1000);
	}
}

Matrix Camera::GetWorldViewPositionMatrix(Matrix World) {
	return World * view * projection;
}
