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

    ~RenderComponent2D();

    int AddRawPoint(Vector4 coords, Color color = Color(1, 1, 1, 1));
    void AddIndex(int index);

    void AddTriangle(Vector4 coords0, Vector4 coords1,
        Vector4 coords2, Color color = Color(1, 1, 1, 1));

    void Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color = Color(1, 1, 1, 1));
    void Add2DCircle(Vector4 centerCoord, float radius, int numberOfTriangles = 16, Color color = Color(1, 1, 1, 1));

    // Inherited via Component
    virtual void Initialize() override;
    virtual void Update() override;

    virtual void Draw() override;    
};

