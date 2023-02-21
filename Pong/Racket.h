#pragma once
#include "GameObject.h"
#include "Keys.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class RenderComponent;
class Ball;

class Racket :
    public GameObject
{
public:
    Racket();

    // Inherited via GameObject
    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;

    Keys upKey = Keys::W;
    Keys downKey = Keys::S;

    DirectX::SimpleMath::Rectangle rect = DirectX::SimpleMath::Rectangle(0, 90, 20, 180);

    Ball* ball;

private:
    RenderComponent* renderComponent;

    float speed = 2;
    
};

