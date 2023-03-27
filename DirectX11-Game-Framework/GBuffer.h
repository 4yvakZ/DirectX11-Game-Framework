#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API GBuffer
{
public:
	GBuffer(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context, Viewport viewport):
		Device(Device),
		Context(Context),
		viewport(viewport)
	{ }

	~GBuffer();


	void Init();

	void Render();

private:

	void CreateDepthBuffer();

	void CreatePositionResources();

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;

	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthView;

	ID3D11Texture2D* positionTexture;
	ID3D11ShaderResourceView* positionSRV;
	ID3D11RenderTargetView* positionRTV;

	Viewport viewport;
};

