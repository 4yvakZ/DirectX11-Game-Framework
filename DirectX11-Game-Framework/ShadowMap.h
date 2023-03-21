#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API ShadowMap
{
	struct CascadeData {
		Matrix viewProjection[4];
		float distances[4];
	};
public:


	ShadowMap(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context, int width, Vector4 lightDir);
	~ShadowMap();

	void Render(ID3D11DeviceContext* Context);
	void Bind(ID3D11DeviceContext* Context);

private:
	void CreateShaderResources(int width, Microsoft::WRL::ComPtr<ID3D11Device>& Device);
	std::vector<Vector4> GetFrustrumCornersWorldSpace(const Matrix& viewProjection);

public:

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;

	ID3D11GeometryShader* geometryShader;
	ID3DBlob* geometryShaderByteCode;

	ID3D11RasterizerState* rastState;

	ID3D11DepthStencilView* DepthView;
	ID3D11Texture2D* shadowMap;
	ID3D11ShaderResourceView* ShadowView;

	ID3D11SamplerState* samplerState;

	Viewport viewport;

	CascadeData cascadeData;
	ID3D11Buffer* cascadeBuffer;

	Vector4 lightDir;
};

