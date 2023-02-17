#pragma once
#include "GameComponent.h"

class GAMEFRAMEWORK_API TriangleComponent:
    public GameComponent
{
public:
    TriangleComponent();

    virtual void DestroyResources() override;

    virtual void Draw() override;

    virtual void Initialize() override;

    virtual void Update() override;

    void SetPoints(DirectX::XMFLOAT4 point1, DirectX::XMFLOAT4 point2, DirectX::XMFLOAT4 point3);

    void SetColor(DirectX::XMFLOAT4 point1Color, DirectX::XMFLOAT4 point2Color, DirectX::XMFLOAT4 point3Color);

    void SetColor(DirectX::XMFLOAT4 Color);

private:
    DirectX::XMFLOAT4 points[6] = {
        DirectX::XMFLOAT4(0.3f, 0.7f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.5f, 0.2f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
    };

    ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;

    ID3D11Buffer* vertices;
    ID3D11Buffer* indexBuffer;

    ID3D11RasterizerState* rastState;
};

