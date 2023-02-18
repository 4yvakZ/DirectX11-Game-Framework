#pragma once
#include "framework.h"

class Component;

class GAMEFRAMEWORK_API GameObject
{
public:
	virtual void Update() = 0;
	virtual void Initialize() = 0;

	~GameObject();

protected:
	std::vector<Component*> components;
};

