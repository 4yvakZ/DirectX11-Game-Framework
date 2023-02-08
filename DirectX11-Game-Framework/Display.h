#pragma once

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define Game_API __declspec(dllexport)
#else
#define Game_API __declspec(dllimport)
#endif

#include <Windows.h>

class Game_API Display
{
public:

	Display();

	Display(int width, int height);

private:
	int width;
	int height;

	WNDCLASSEX wc;

	HINSTANCE hInstance;

	HWND hWnd;
};

