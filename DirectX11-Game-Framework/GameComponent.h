#pragma once

#include "Game.h"

#ifdef DIRECTX11GAMEFRAMEWORK_EXPORTS
#define Game_API __declspec(dllexport)
#else
#define Game_API __declspec(dllimport)
#endif

class Game;

class Game_API GameComponent
{
public:

	virtual void DestroyResources() = 0;

	virtual void Draw() = 0;

	GameComponent() {};

	virtual void Initialize();

	virtual void Reload() {};

	virtual void Update() = 0;

	Game* game;
};

