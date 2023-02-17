#pragma once

#include "framework.h"

#include "Component.h"

class GAMEFRAMEWORK_API RenderComponent :
    public Component
{
public:
    RenderComponent(std::string shaderFileName) :
        Component(),
        shaderFileName(shaderFileName)
    {

    }

    RenderComponent() = delete;

    int AddPoint(Vector4 coords, Color color = Color(1, 1, 1, 1));
    void AddIndex(int index);

    void Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color = Color(1, 1, 1, 1));
    void Add2DCircle(Vector4 centerCoord, float radius, Color color = Color(1, 1, 1, 1));

    // Inherited via Component
    virtual void Initialize() override;
    virtual void Update() override;

    void Draw();

private:
    std::string shaderFileName;

    std::vector<Vector4> points;
    std::vector<int> indexes;
    

    ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;

    ID3D11Buffer* vertices;
    ID3D11Buffer* indexBuffer;

    ID3D11RasterizerState* rastState;
};

