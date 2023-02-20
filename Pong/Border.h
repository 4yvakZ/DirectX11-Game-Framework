#pragma once

#include "GameObject.h"

class RenderComponent;

class Border :
	public GameObject
{
public:
	Border();
	// Inherited via GameObject
	virtual void Update() override;
	virtual void Initialize() override;

private:
	RenderComponent* renderComponent;

};

