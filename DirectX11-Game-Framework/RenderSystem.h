#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class DisplayWin;
class RenderComponent;
class ShadowMap;

class GAMEFRAMEWORK_API RenderSystem
{

public:
	struct LightData
	{
		Vector4 direction;
		Vector4 intensity{ 1.5f, 1.5f, 1.5f, 0.0f };
	};

	RenderSystem(DisplayWin* display);
	RenderSystem() = delete;

	~RenderSystem();

	virtual void Draw();

	virtual void EndFrame();

	virtual void PrepareFrame();

	void RemoveRenderComponent(RenderComponent* renderComponent);

	void UpdateLightData(LightData newLightData);

protected:

	void CreateBackBuffer();

	void CreateDepthBuffer();

	void CreateLightBuffer();

	void CreateShadowMap();

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;

	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* DepthView;

	ShadowMap* shadowMap;

	Viewport viewport;

	std::vector <RenderComponent*> renderComponents;

	DisplayWin* display;

	LightData lightData;
	ID3D11Buffer* lightBuffer;
};

