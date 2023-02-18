#include "GameObject.h"

GameObject::~GameObject()
{
	for (auto component : components) {
		delete component;
	}
	components.clear();
}
