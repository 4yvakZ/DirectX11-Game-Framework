#pragma once

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define Game_API __declspec(dllexport)
#else
#define Game_API __declspec(dllimport)
#endif

#include <Windows.h>

class Game_API DisplayWin
{
public:

	DisplayWin();

	DisplayWin(int ClientWidth, int ClientHeight);

public:
	int ClientWidth;
	int ClientHeight;

	WNDCLASSEX wc;

	HINSTANCE hInstance;

	HWND hWnd;
};

