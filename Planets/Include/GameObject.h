#pragma once

#include "framework.h"

class Component;

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API GameObject
{
public:
	GameObject(GameObject* parent = nullptr);

	virtual void Update(float deltaTime);
	virtual void Initialize();

	virtual ~GameObject();

	GameObject* parent = nullptr;

	virtual void SetWorld(const Vector3& newPosition, const Quaternion& newRotation);
	virtual Matrix GetWorld() const;
	
	virtual void SetRotation(const Quaternion &newRotation);
	virtual Quaternion GetRotation() const;
	virtual void SetPosition(const Vector3& newPosition);
	virtual Vector3 GetPosition() const;

protected:
	std::vector<Component*> components;
	virtual void UpdateWorld();
	Matrix world;

private:
	Quaternion rotation;
	Vector3 position;
};