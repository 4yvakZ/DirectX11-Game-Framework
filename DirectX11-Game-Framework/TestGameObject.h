#pragma once
#include "GameObject.h"

class RenderComponent;

class GAMEFRAMEWORK_API TestGameObject :
    public GameObject
{
public:
    TestGameObject();

    // Inherited via GameObject
    virtual void Update() override;

    virtual void Initialize() override;

private:

    RenderComponent* renderComponent;

};

