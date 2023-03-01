#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(GameObject* parent):
	parent(parent)
{
	
}

void GameObject::Update(float deltaTime)
{
	for (const auto& component : components) {
		component->Update();
	}
}

void GameObject::Initialize()
{
	for (const auto& component : components) {
		component->Initialize();
	}
}

GameObject::~GameObject()
{
	for (const auto& component : components) {
		delete component;
	}
	components.clear();
}


void GameObject::SetWorld(const Vector3& newPosition, const Quaternion& newRotation)
{
	rotation = newRotation;
	position = newPosition;
	UpdateWorld();
}

Matrix GameObject::GetWorld() const
{
	return world;
}

void GameObject::SetRotation(const Quaternion& newRotation)
{
	rotation = newRotation;
	UpdateWorld();
}

Quaternion GameObject::GetRotation() const
{
	return rotation;
}

void GameObject::SetPosition(const Vector3& newPosition)
{
	position = newPosition;
	UpdateWorld();
}

Vector3 GameObject::GetPosition() const
{
	return position;
}

void GameObject::UpdateWorld()
{
	world = Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
	if (parent) {
		world *= parent->GetWorld();
	}
}
