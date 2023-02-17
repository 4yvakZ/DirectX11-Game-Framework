#pragma once

#include "framework.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "DisplayWin.h"
#include "RenderSystem.h"
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

	virtual void Initialize();

	virtual void PrepareResources();

	virtual void Update();

	virtual void UpdateInternal();

private:
	Game();

	static RenderSystem* render;

	static Game* instance;

	static DisplayWin* display;

public:

	static Game* GetInstance() {
		return instance;
	}

	static RenderSystem* GetRenderSystem() {
		return render;
	}

	static DisplayWin* GetDisplay() {
		return display;
	}

	std::vector<GameComponent*> Components;

	std::string Name;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	int StartTime;
	float TotalTime;

	DirectX::SimpleMath::Rectangle rect;

private:
};

