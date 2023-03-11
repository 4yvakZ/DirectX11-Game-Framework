#pragma once
#include "GameObject.h"

class RenderComponent2D;
class RenderComponentFBX;
class LineRenderComponent;

class GAMEFRAMEWORK_API TestGameObject :
    public GameObject
{
public:
    TestGameObject();

    // Inherited via GameObject
    virtual void Update(float deltaTime) override;
    virtual void Initialize() override;



private:

    RenderComponent2D* renderComponent;
    RenderComponentFBX* FbxRenderComponent;

    float speed = 1.0f;
};

