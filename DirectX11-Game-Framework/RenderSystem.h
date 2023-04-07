#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class DisplayWin;
class RenderComponent;
class ShadowMap;
class DirectionalLight;
class GBuffer;
class ParticleSystem;

constexpr int kTreadGruopX = 32;
constexpr int kTreadGruopY = 32;

class GAMEFRAMEWORK_API RenderSystem
{

public:
	RenderSystem(DisplayWin* display);
	RenderSystem() = delete;

	~RenderSystem();

	virtual void Draw();

	virtual void EndFrame();

	virtual void PrepareFrame();

	void RemoveRenderComponent(RenderComponent* renderComponent);

protected:

	void CreateBackBuffer();

	void CreateDepthStencilState();

	void CreateLight(Vector4 LightDir);

	void CreateLightShader();

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;

	ID3D11Texture2D* lightTexture;
	ID3D11ShaderResourceView* lightSRV;
	ID3D11RenderTargetView* lightRTV;

	ID3D11DepthStencilState* depthStencilStateOff;
	ID3D11BlendState* blendState;

	ID3D11VertexShader* lightVertexShader;
	ID3DBlob* lightVertexShaderByteCode;
	ID3D11PixelShader* lightPixelShader;
	ID3DBlob* lightPixelShaderByteCode;

	ID3D11VertexShader* colorVertexShader;
	ID3DBlob* colorVertexShaderByteCode;
	ID3D11PixelShader* colorPixelShader;
	ID3DBlob* colorPixelShaderByteCode;

	std::vector<ShadowMap*> shadowMaps;
	std::vector<DirectionalLight*> dirLights;

	Viewport viewport;

	std::vector <RenderComponent*> renderComponents;

	DisplayWin* display;

	GBuffer* gBuffer; 
	ID3D11RasterizerState* rastState;

	ParticleSystem* particleSystem;
};

