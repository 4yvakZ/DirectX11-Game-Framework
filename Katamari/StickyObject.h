#pragma once
#include "GameObject.h"

class RenderComponentFBX;
class LineRenderComponent;
using namespace DirectX::SimpleMath;

class StickyObject :
    public GameObject
{
public:
    StickyObject(const std::string& modelFileName, const std::string& textureFileName = "");

    virtual void Update(float deltaTime);
    virtual void Initialize();

    void Attach(GameObject* gameObject);
    void SetCenterPoint(Vector3 newCenterPoint);

public:
    float radius;

private:
    RenderComponentFBX* FbxRenderComponent;
    LineRenderComponent* lineRenderer;
    Vector3 centerPoint;
};

