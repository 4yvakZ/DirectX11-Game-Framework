#pragma once
#include "CameraController.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API SphereCameraController :
    public CameraController
{
public:

    SphereCameraController(Camera* camera);
    // Inherited via CameraController
    virtual void Update(float deltaTime) override;
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload) override;

    float cameraSpeed = 5.0f;
    float cameraRotationSpeed = 0.005f;

    float armLength = 5.0f;
    float yaw = 0;
    float pitch = 0;
};

