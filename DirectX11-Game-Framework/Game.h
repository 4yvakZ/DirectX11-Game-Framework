#pragma once

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define Game_API __declspec(dllexport)
#else
#define Game_API __declspec(dllimport)
#endif

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


#include "DisplayWin.h"
#include "GameComponent.h"

class GameComponent;

class Game_API Game
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
	int RenderSRV;

	std::vector<GameComponent*> Components;

	int DebugAnnotation;

	std::string Name;
	int prevTime;
	int StartTime;
	int TotalTime;
	bool ScreenResized;
};
