#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class DisplayWin;
class RenderComponent;

class RenderSystem
{

public:


	struct LightData 
	{
		Vector4 direction;
		Vector4 intensity{ 1.0f, 1.0f, 1.0f, 1.0f};
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

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;

	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* DepthView;

	Viewport viewport;

	std::vector <RenderComponent*> renderComponents;

	DisplayWin* display;

private:
	LightData lightData;
	ID3D11Buffer* lightBuffer;
};

