#pragma once

#include "RenderComponent.h"

class GAMEFRAMEWORK_API RenderComponentFBX :
	public RenderComponent
{
public:
	RenderComponentFBX(std::string shaderFileName, const char* fileName);

	RenderComponentFBX() = delete;

	// Inherited via RenderComponent
	virtual void Initialize() override;
	virtual void Draw() override;
	virtual void Update() override;

private:
	const char* fileName;
};

