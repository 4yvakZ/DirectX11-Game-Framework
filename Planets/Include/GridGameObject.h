#pragma once
#include "GameObject.h"

class LineRenderComponent;

class GAMEFRAMEWORK_API GridGameObject :
    public GameObject
{
public:
    GridGameObject();

    // Inherited via GameObject
    virtual void Update(float deltaTime) override;

    virtual void Initialize() override;

private:
    LineRenderComponent* gridRenderComponent;
};

