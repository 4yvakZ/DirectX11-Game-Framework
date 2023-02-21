#include "GameObject.h"
#include "Component.h"

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
