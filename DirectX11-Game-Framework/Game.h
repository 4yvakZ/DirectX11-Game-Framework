#pragma once

#include "framework.h"

class DisplayWin;
class GameObject;
class RenderSystem;
class InputDevice;
class Camera;

class FPSCameraController;
class SphereCameraController;

class GAMEFRAMEWORK_API Game
{
public:
	Game();

	void Exit();

	void RestoreTarget();

	void Run();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

protected:

	virtual void DestroyResources();

	virtual void Draw();

	virtual void Initialize();

	virtual void PrepareResources();

	virtual void Update();

	virtual void UpdateInternal();

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

	static InputDevice* GetInputDevice() {
		return inputDevice;
	}

	static Camera* GetCamera() {
		return camera;
	}

	std::vector<GameObject*> GameObjects;

private:

	std::string Name;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	int startTime;
	float totalTime;
	float deltaTime;

	bool isExitRequested = false;

	static RenderSystem* render;

	static Game* instance;

	static DisplayWin* display;

	static InputDevice* inputDevice;

	static Camera* camera;

	FPSCameraController* fpsCameraContrloller;
	SphereCameraController* sphereCameraController;

	bool wasProjectionKeyDown = false;
	bool wasCameraControllerKeyDown = false;
};


