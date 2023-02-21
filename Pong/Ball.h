#pragma once
#include "GameObject.h"
#include "Counter.h"

class RenderComponent;

class Ball :
    public GameObject
{
public:
    Ball();

    // Inherited via GameObject
    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;

    void Bounce(float yVelocityDirection);

    int x;
    int y;
    int radius = 15;

private:

    RenderComponent* renderComponent;
    Counter counter;

    float speed = 1.5;
    float xv = 0.7;
    float yv = 0.7;
};

