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


void GameObject::SetWorld(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	transform.rotation = rotation;
	transform.position = position;
	transform.scale = scale;
	UpdateWorld();
}

void GameObject::SetWorld(Matrix newWorld)
{
	if (parent) {
		auto pWorld = parent->GetWorld();
		pWorld.Invert();
		newWorld *= pWorld;
	}
	newWorld.Decompose(transform.scale, transform.rotation, transform.position);
	UpdateWorld();
}

Matrix GameObject::GetWorld()
{
	UpdateWorld();
	return transform.world;
}

void GameObject::SetRotation(const Quaternion& rotation)
{
	transform.rotation = rotation;
	UpdateWorld();
}

Quaternion GameObject::GetRotation() const
{
	return transform.rotation;
}

void GameObject::SetPosition(const Vector3& position)
{
	transform.position = position;
	UpdateWorld();
}

Vector3 GameObject::GetPosition() const
{
	return transform.position;
}

void GameObject::SetScale(const Vector3& scale)
{
	transform.scale = scale;
	UpdateWorld();
}

void GameObject::SetScale(float scale)
{
	transform.scale = Vector3(scale, scale, scale);
	UpdateWorld();
}

Vector3 GameObject::GetScale() const
{
	return transform.scale;
}

void GameObject::UpdateWorld()
{
	transform.world = Matrix::CreateFromQuaternion(transform.rotation) * Matrix::CreateScale(transform.scale) * Matrix::CreateTranslation(transform.position);
	if (parent) {
		transform.world *= parent->GetWorld();
	}
}
