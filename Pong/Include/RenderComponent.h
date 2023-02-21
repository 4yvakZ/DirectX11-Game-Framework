#pragma once

#include "Component.h"

using namespace DirectX::SimpleMath;

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

    ~RenderComponent();

    int AddRawPoint(Vector4 coords, Color color = Color(1, 1, 1, 1));
    void AddIndex(int index);

    void AddTriangle(Vector4 coords0, Vector4 coords1,
        Vector4 coords2, Color color = Color(1, 1, 1, 1));

    void Add2DRect(DirectX::SimpleMath::Rectangle rect, Color color = Color(1, 1, 1, 1));
    void Add2DCircle(Vector4 centerCoord, float radius, int numberOfTriangles = 16, Color color = Color(1, 1, 1, 1));

    // Inherited via Component
    virtual void Initialize() override;
    virtual void Update() override;

    void Draw();

    Vector4 offset;

private:
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

