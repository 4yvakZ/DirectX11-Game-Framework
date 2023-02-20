#pragma once
#include "GameObject.h"

class RenderComponent;

class Racket :
    public GameObject
{
public:
    Racket();

    // Inherited via GameObject
    virtual void Update() override;
    virtual void Initialize() override;

private:
    RenderComponent* renderComponent;
};

