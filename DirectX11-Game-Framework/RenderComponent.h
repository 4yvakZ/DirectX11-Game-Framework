#pragma once
#include "Component.h"
#include <vector>

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API RenderComponent :
    public Component
{
public:
    RenderComponent (std::string shaderFileName) :
        shaderFileName(shaderFileName),
        Component() {}

    RenderComponent() = delete;

    virtual void Draw() = 0;

    // Inherited via Component
    virtual void Initialize();
    virtual void Update() = 0;

public:
    Vector4 offset;

protected:
    std::string shaderFileName;

    std::vector<Vector4> points;
    std::vector<int> indexes;


    ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;

    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* constBuffer;

    ID3D11RasterizerState* rastState;
};

