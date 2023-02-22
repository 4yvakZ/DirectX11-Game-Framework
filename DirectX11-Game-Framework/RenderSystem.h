#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class DisplayWin;
class RenderComponent;

class RenderSystem
{

public:
	RenderSystem(DisplayWin* display);
	RenderSystem() = delete;

	~RenderSystem();

	virtual void Draw();

	virtual void EndFrame();

	virtual void PrepareFrame();

	void RemoveRenderComponent(RenderComponent* renderComponent);

private:

	void CreateBackBuffer();

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;

	Viewport viewport;

	std::vector <RenderComponent*> renderComponents;
};

