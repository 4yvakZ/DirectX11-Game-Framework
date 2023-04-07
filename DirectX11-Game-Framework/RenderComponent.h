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
        Matrix view;
        Matrix projection;
        Matrix world;
        Vector4 cameraPos;
    };

    struct MaterialBufferData
    {
        Vector4 ambient = Vector4(0.2f, 0.2f, 0.2f, 0.0f);
        Vector4 diffuse = Vector4(0.5f, 0.5f, 0.5f, 0.0f);
        Vector4 specularAlpha = Vector4(0.9f, 0.9f, 0.9f, 20.0f);
    };

    struct VertexData 
    {
        VertexData(Vector3 pos, Vector2 uv, Vector3 normal) :
            pos(pos), color(Color(1, 1, 1, 1)), uv(uv), normal(normal) {}

        VertexData(Vector3 pos, Color color = Color(1, 1, 1, 1)) :
            pos(pos), color(color), uv(0,0), normal(0,0,-1) {}
        
        Vector3 pos;
        Color color;
        Vector2 uv;
        Vector3 normal;
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
    MaterialBufferData materialBufferData;
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
    ID3D11Buffer* materialBuffer;

    ID3D11RasterizerState* rastState;

    ID3D11ShaderResourceView* textureView = nullptr;
    ID3D11SamplerState* samplerState;
};

