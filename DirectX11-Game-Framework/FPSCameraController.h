#pragma once
#include "CameraController.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API FPSCameraController :
    public CameraController
{
public:
    FPSCameraController(Camera* camera);


    // Inherited via CameraController
    virtual void Update(float deltaTime) override;
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload) override;

    float cameraSpeed = 4.0f;
    float cameraRotationSpeed = 0.005f;
    float cameraFOVSpeed = 0.001f;

    float yaw = 0;
    float pitch = 0;
};

