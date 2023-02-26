#pragma once

#include "framework.h"

class Component;

class GAMEFRAMEWORK_API GameObject
{
public:
	virtual void Update(float deltaTime);
	virtual void Initialize();

	virtual ~GameObject();

protected:
	std::vector<Component*> components;
};