#include "Game.h"

#include "RenderSystem.h"
#include "DisplayWin.h"
#include "GameObject.h"

Game* Game::instance = new Game();

RenderSystem* Game::render = nullptr;
DisplayWin* Game::display = nullptr;

Game::Game()
{

}

void Game::Initialize()
{
	display = new DisplayWin{1200, 800};

	render = new RenderSystem{ display };

	for (auto& object : GameObjects) {
		object->Initialize();
	}
}


void Game::RestoreTarget()
{
}

void Game::Run()
{
	Initialize();

	PrepareResources();

	PrevTime = std::chrono::steady_clock::now();
	TotalTime = 0;
	unsigned int frameCount = 0;

	MSG msg;

	bool isExitRequested = false;
	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		auto	curTime = std::chrono::steady_clock::now();
		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
		PrevTime = curTime;

		TotalTime += deltaTime;
		frameCount++;

		if (TotalTime > 1.0f) {
			float fps = frameCount / TotalTime;

			TotalTime -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(display->hWnd, text);

			frameCount = 0;
		}

		Update();

		Draw();
	}

	DestroyResources();
}

void Game::DestroyResources()
{

}

void Game::Draw()
{
	render->PrepareFrame();

	render->Draw();

	render->EndFrame();
}

void Game::PrepareResources()
{
}

void Game::Update()
{
	UpdateInternal();

	for (auto& object : GameObjects) {
		object->Update();
	}
}

void Game::UpdateInternal()
{
}

void Game::Exit()
{
	delete render;
	delete display;
	delete instance;
}
