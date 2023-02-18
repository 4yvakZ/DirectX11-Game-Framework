#pragma once
#include "framework.h"

class GAMEFRAMEWORK_API DisplayWin
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

