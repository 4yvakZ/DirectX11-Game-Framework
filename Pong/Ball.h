#pragma once
#include "GameObject.h"

class RenderComponent;

class Ball :
    public GameObject
{
public:
    Ball();

    // Inherited via GameObject
    virtual void Update() override;
    virtual void Initialize() override;

private:
    RenderComponent* renderComponent;


};

