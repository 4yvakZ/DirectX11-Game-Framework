#pragma once

#include "Game.h"
#include "RenderSystem.h"

class Game;

class GAMEFRAMEWORK_API GameComponent
{
public:

	virtual void DestroyResources() = 0;

	virtual void Draw() = 0;

	virtual void Initialize() = 0;

	virtual void Reload() {};

	virtual void Update() = 0;
};

