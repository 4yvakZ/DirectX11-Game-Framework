#pragma once
#include "CameraController.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API FPSCameraController :
    public CameraController
{
public:
    FPSCameraController(Camera* camera) :
        CameraController(camera) {}


    // Inherited via CameraController
    virtual void Update(float deltaTime) override;


    float cameraSpeed = 4.0f;
};

