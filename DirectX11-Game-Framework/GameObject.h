#pragma once

#include "framework.h"
#include "Transform.h"

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

	virtual void SetWorld(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	
	virtual Matrix GetWorld();
	
	virtual void SetRotation(const Quaternion &rotation);
	virtual Quaternion GetRotation() const;
	virtual void SetPosition(const Vector3& position);
	virtual Vector3 GetPosition() const;
	void SetScale(float scale);
	virtual Vector3 GetScale() const;

protected:
	virtual void UpdateWorld();

private:
	//Cursed functions
	virtual void SetScale(const Vector3& scale);
	virtual void SetWorld(Matrix newWorld);

protected:
	std::vector<Component*> components;
	
private:
	Transform transform;
};