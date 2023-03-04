#pragma once

#include "RenderComponent.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API RenderComponent2D :
    public RenderComponent
{
public:
    RenderComponent2D(std::string shaderFileName) :
        RenderComponent(shaderFileName)
    {

    }

    RenderComponent2D() = delete;

    int AddRawPoint(Vector3 coords, Color color = Color(1, 1, 1, 1));
    void AddIndex(int index);

    void AddTriangle(Vector3 coords0, Vector3 coords1,
        Vector3 coords2, Color color = Color(1, 1, 1, 1));

    void Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color = Color(1, 1, 1, 1));
    void Add2DCircle(Vector3 centerCoord, float radius, int numberOfTriangles = 16, Color color = Color(1, 1, 1, 1));


    virtual void Initialize() override;
    virtual void Update() override;

    virtual void Draw() override;    
};

