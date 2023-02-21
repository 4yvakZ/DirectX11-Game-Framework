#include "GameObject.h"
#include "Component.h"

void GameObject::Update(float deltaTime)
{
	for (auto component : components) {
		component->Update();
	}
}

void GameObject::Initialize()
{
	for (auto component : components) {
		component->Initialize();
	}
}

GameObject::~GameObject()
{
	for (auto component : components) {
		delete component;
	}
	components.clear();
}
