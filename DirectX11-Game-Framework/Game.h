#pragma once

#include "framework.h"

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "SimpleMath.h"

#include "DisplayWin.h"
#include "GameComponent.h"

class GameComponent;

class GAMEFRAMEWORK_API Game
{
public:
	void Exit();

	void RestoreTarget();

	void Run();

protected:

	virtual void DestroyResources();

	virtual void Draw();

	virtual void EndFrame();

	virtual void Initialize();

	virtual void PrepareFrame();

	virtual void PrepareResources();

	virtual void Update();

	virtual void UpdateInternal();

private:
	Game();

	void CreateBackBuffer();

public:

	static Game* Instance;

	DisplayWin* Display;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* RenderView;

	std::vector<GameComponent*> Components;

	std::string Name;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	int StartTime;
	float TotalTime;

	DirectX::SimpleMath::Rectangle rect;
};

