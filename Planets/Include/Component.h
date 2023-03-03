#pragma once

#include "framework.h"

class GAMEFRAMEWORK_API Component
{
public:
	virtual ~Component() {}

	virtual void Initialize() = 0;

	virtual void Update() = 0;
};

