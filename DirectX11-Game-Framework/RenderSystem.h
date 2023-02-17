#pragma once

#include "framework.h"


#include "DisplayWin.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API RenderSystem
{

public:
	RenderSystem(DisplayWin* display);
	RenderSystem() = delete;

	~RenderSystem();

	virtual void Draw();

	virtual void EndFrame();

	virtual void PrepareFrame();

private:

	void CreateBackBuffer();

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;
};

