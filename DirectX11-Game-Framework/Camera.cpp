#include "Camera.h"
#include "Game.h"
#include "DisplayWin.h"

Camera::Camera()
{
	position = Vector3(0.0f, 0.0f, -0.5f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
	view = Matrix::CreateLookAt(position, target, up);

	int width = Game::GetDisplay()->ClientWidth;
	int height = Game::GetDisplay()->ClientHeight;
	
	if (isPerspectiveProjection) {
		projection = Matrix::CreatePerspectiveFieldOfView(
			DirectX::XM_PIDIV2,
			static_cast<float>(width) / height,
			1.0f,
			1000.0f);
	}
	else {
		projection = Matrix::CreateOrthographic(
			width,
			height,
			0.1,
			1000);
	}
}

Matrix Camera::GetWorldViewPositionMatrix(Matrix World) {
	return World * view * projection;
}
