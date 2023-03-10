#pragma once
#include "GameObject.h"
#include <DirectXCollision.h>

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
    void UpdateCollision();
    void UpdatePosition(float deltaTime);

private:
    float rotationSpeed = 5;
    RenderComponentFBX* FbxRenderComponent;
    float radius;
    float radiusIncreaseSpeed = 0.5;
    float targetRadius;

    DirectX::BoundingSphere boundingSphere;
};

