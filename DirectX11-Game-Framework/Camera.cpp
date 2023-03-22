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
	aspectRatio = static_cast<float>(width) / height;
	ortWidth = width / 50;
	ortHeight = height / 50;

	
	if (isPerspectiveProjection) {
		projection = Matrix::CreatePerspectiveFieldOfView(
			fovAngle,
			aspectRatio,
			nearPlane,
			farPlane);
	}
	else {
		projection = Matrix::CreateOrthographic(
			ortWidth,
			ortHeight,
			nearPlane,
			farPlane);
	}
	//projection = Matrix::Identity;
}

Matrix Camera::GetWorldViewPositionMatrix(Matrix World) {
	return World * view * projection;
}

Vector3 Camera::GetForwardVector()
{
	if (controller)
	{
		return controller->GetForwardVector();
	}
	return Vector3::Forward;
}

Vector3 Camera::GetRightVector()
{
	if (controller)
	{
		return controller->GetRightVector();
	}
	return Vector3::Right;
}

Vector3 Camera::GetUpVector()
{
	if (controller)
	{
		return controller->GetUpVector();
	}
	return Vector3::Up;
}
