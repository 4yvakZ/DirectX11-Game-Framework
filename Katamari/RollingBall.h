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

    float rotationSpeed = 3;

private:
    RenderComponentFBX* FbxRenderComponent;
    float radius;
};

