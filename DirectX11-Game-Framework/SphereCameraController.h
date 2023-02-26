#pragma once
#include "CameraController.h"

class SphereCameraController :
    public CameraController
{
public:

    SphereCameraController(Camera* camera);
    // Inherited via CameraController
    virtual void Update(float deltaTime) override;
    virtual void MouseEventHandler(const InputDevice::MouseMoveEventArgs& mouseData, int payload) override;
};

