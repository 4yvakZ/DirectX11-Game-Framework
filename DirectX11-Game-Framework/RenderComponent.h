#pragma once
#include "Component.h"
#include <vector>

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API RenderComponent :
    public Component
{
public:

    struct ConstBufferData 
    {
        Matrix worldViewPosition;
    };

    struct VertexData 
    {
        Vector3 pos;
        Color color;
        Vector2 uv;
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

    std::vector<VertexData> points;
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

    ID3D11ShaderResourceView* textureView = nullptr;
    ID3D11SamplerState* samplerState;
};

