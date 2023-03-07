#pragma once
#include "GameObject.h"

class RenderComponentFBX;

class PlaneObject :
    public GameObject
{
public:
    PlaneObject();

    // Inherited via GameObject
    virtual void Update(float deltaTime) override;

    virtual void Initialize() override;

private:
    RenderComponentFBX* FbxRenderComponent;
};

