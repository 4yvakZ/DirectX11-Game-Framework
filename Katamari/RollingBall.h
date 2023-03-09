#pragma once
#include "GameObject.h"

class RenderComponentFBX;
using namespace DirectX::SimpleMath;

class RollingBall :
    public GameObject
{
public:
    RollingBall(float radius, GameObject* parent = nullptr);
    virtual ~RollingBall();

    virtual void Update(float deltaTime);
    virtual void Initialize();

    void IncreaseRadius(float additionalRadius);

   

private:

    float rotationSpeed = 3;
    RenderComponentFBX* FbxRenderComponent;
    float radius;
    float radiusIncreaseSpeed = 0.5;
    float targetRadius;
};

