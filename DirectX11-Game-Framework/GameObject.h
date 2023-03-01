#pragma once

#include "framework.h"

class Component;

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API GameObject
{
public:
	virtual void Update(float deltaTime);
	virtual void Initialize();

	virtual ~GameObject();

	GameObject* parent = nullptr;
	Matrix World;

protected:
	std::vector<Component*> components;
};