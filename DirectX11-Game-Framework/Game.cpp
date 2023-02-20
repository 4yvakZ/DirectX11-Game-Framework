#include "Game.h"

#include "RenderSystem.h"
#include "DisplayWin.h"
#include "InputDevice.h"

#include "GameObject.h"

Game* Game::instance = new Game();

RenderSystem* Game::render = nullptr;
DisplayWin* Game::display = nullptr;
InputDevice* Game::inputDevice = nullptr;

LRESULT Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	{
		switch (umessage)
		{
		case WM_INPUT:
		{
			UINT dwSize = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == nullptr) {
				return 0;
			}

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				//	raw->data.keyboard.MakeCode,
				//	raw->data.keyboard.Flags,
				//	raw->data.keyboard.Reserved,
				//	raw->data.keyboard.ExtraInformation,
				//	raw->data.keyboard.Message,
				//	raw->data.keyboard.VKey);

				inputDevice->OnKeyDown({
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.VKey,
					raw->data.keyboard.Message
					});
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				inputDevice->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
			}

			delete[] lpb;
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		}
	}
}


Game::Game()
{

}

void Game::Initialize()
{
	for (auto& object : GameObjects) {
		object->Initialize();
	}
}


void Game::RestoreTarget()
{
}

void Game::Run()
{
	PrepareResources();

	Initialize();

	PrevTime = std::chrono::steady_clock::now();
	TotalTime = 0;
	unsigned int frameCount = 0;

	MSG msg;

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
	delete render;
	delete display;
	delete inputDevice;
	for (auto& object : GameObjects) {
		object->Update();
	}
	GameObjects.clear();
	delete instance;
}

void Game::Draw()
{
	render->PrepareFrame();

	render->Draw();

	render->EndFrame();
}

void Game::PrepareResources()
{
	display = new DisplayWin{ 1200, 800 };

	render = new RenderSystem{ display };

	inputDevice = new InputDevice();
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
	if (inputDevice->IsKeyDown(Keys::Escape)) {
		std::cout << "Escape pressed\n";
		Exit();
	}
	if (inputDevice->IsKeyDown(Keys::A)) {
		std::cout << "A pressed\n";
	}
}

void Game::Exit()
{
	isExitRequested = true;
}
