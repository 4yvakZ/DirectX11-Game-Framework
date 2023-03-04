#pragma once
#include "Component.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API RenderComponent :
    public Component
{
public:
    struct ConstBufferData {
        Matrix worldViewPosition;
    };

    RenderComponent (std::string shaderFileName) :
        shaderFileName(shaderFileName),
        Component() {}

    RenderComponent() = delete;

    virtual ~RenderComponent();

    virtual void Draw() = 0;

    // Inherited via Component
    virtual void Initialize();
    virtual void Update() = 0;

protected:
    void UpdateConstBuffer();

public:
    ConstBufferData constBufferData;
    Matrix World;

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

