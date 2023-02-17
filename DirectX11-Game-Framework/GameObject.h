#pragma once
#include "framework.h"
#include "Component.h"

class GAMEFRAMEWORK_API GameObject
{
public:
	virtual void Update() = 0;
	virtual void Initialize() = 0;

protected:
	std::vector<Component*> components;
};

