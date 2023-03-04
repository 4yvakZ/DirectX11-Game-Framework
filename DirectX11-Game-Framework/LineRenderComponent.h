#pragma once
#include "RenderComponent.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API LineRenderComponent :
    public RenderComponent
{
public:
    LineRenderComponent(std::string shaderFileName) :
        RenderComponent(shaderFileName) {};

    LineRenderComponent() = delete;

    void AddIndex(int index);
    void AddLine(Vector3 coords0, Vector3 coords1, Color color);
    void AddGrid(int gridSize, float cellSize, Color color);

    // Inherited via RenderComponent
    virtual void Draw() override;
    virtual void Update() override;
};

