#include "SpaceObject.h"
#include "RenderComponentFBX.h"

SpaceObject::SpaceObject(bool isCube, GameObject* parent):
	GameObject(parent)
{
	if (isCube) 
	{
		FbxRenderComponent = new RenderComponentFBX("../Shaders/MyVeryFirstShader.hlsl", "../FBX/cube.fbx");
	}
	else
	{
		FbxRenderComponent = new RenderComponentFBX("../Shaders/MyVeryFirstShader.hlsl", "../FBX/sphere.fbx");
	}
	
	components.push_back(FbxRenderComponent);
}

void SpaceObject::Update(float deltaTime)
{
	rotator *= Quaternion::CreateFromAxisAngle(rotationAxis, rotationSpeed * deltaTime);
	orbitalRotation *= Quaternion::CreateFromAxisAngle(orbitAxis, orbitSpeed * deltaTime);

	SetRotation(rotator);

	FbxRenderComponent->World = GetWorld();
	GameObject::Update(deltaTime);
}

void SpaceObject::Initialize()
{
	if (orbitAxis.Dot(GameObject::GetPosition()) != 0) 
	{
		Vector3 normal = orbitAxis.Cross(GameObject::GetPosition());
		orbitAxis = normal.Cross(GameObject::GetPosition());
	}

	orbitAxis.Normalize();
	rotationAxis.Normalize();
	
	GameObject::Initialize();
}

Vector3 SpaceObject::GetPosition() const
{
	if (parent) 
	{
		return Vector3::Transform(GameObject::GetPosition(), Matrix::CreateFromQuaternion(orbitalRotation) * Matrix::CreateTranslation(parent->GetPosition()));
	}
	return GameObject::GetPosition();
}

void SpaceObject::UpdateWorld()
{
	world = Matrix::CreateFromQuaternion(GetRotation()) * Matrix::CreateTranslation(GameObject::GetPosition());
	if (parent) 
	{
		world *= Matrix::CreateFromQuaternion(orbitalRotation) * Matrix::CreateTranslation(parent->GetPosition());
	}
}
