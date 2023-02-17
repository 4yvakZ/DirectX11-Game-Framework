#pragma once

#include "Game.h"

class Game;

class GAMEFRAMEWORK_API GameComponent
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

