#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API ShadowMap
{
public:
	ShadowMap(Microsoft::WRL::ComPtr<ID3D11Device> Device, int width);
	~ShadowMap();

	void Render(ID3D11DeviceContext* Context);
	void Bind(ID3D11DeviceContext* Context);

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;

	ID3D11RasterizerState* rastState;

	ID3D11DepthStencilView* DepthView;
	ID3D11Texture2D* shadowMap;
	ID3D11ShaderResourceView* ShadowView;

	ID3D11SamplerState* samplerState;

	Viewport viewport;
};

