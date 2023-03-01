#pragma once
#include "GameObject.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class RenderComponentFBX;

class SpaceObject :
    public GameObject
{
public:
    SpaceObject(bool isCube, GameObject* parent = nullptr);

    virtual void Update(float deltaTime) override;

    virtual void Initialize() override;



public:

    virtual Vector3 GetPosition() const override;

    Vector3 rotationAxis = Vector3(0, 1, 0);
    Vector3 orbitAxis = Vector3(0, 1, 0);
    Vector3 currentPos;
    
    float rotationSpeed = 1;
    float orbitSpeed = -1;

protected:
    virtual void UpdateWorld() override;

private:
    RenderComponentFBX* FbxRenderComponent;
    Quaternion rotator;
    Quaternion orbitalRotation;

};

