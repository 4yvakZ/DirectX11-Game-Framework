#include "StickyObject.h"
#include "RenderComponentFBX.h"
#include "LineRenderComponent.h"

StickyObject::StickyObject(const std::string& modelFileName, const std::string& textureFileName)
	:GameObject()
{
	FbxRenderComponent = new RenderComponentFBX("../Shaders/SimpleTextureShader.hlsl", modelFileName, textureFileName);
	components.push_back(FbxRenderComponent);
	//lineRenderer = new LineRenderComponent("../Shaders/SimpleTextureShader.hlsl");
	//components.push_back(lineRenderer);
}

void StickyObject::Update(float deltaTime)
{
	FbxRenderComponent->World = GetWorld();
	//lineRenderer->World = GetWorld();
	GameObject::Update(deltaTime);
}

void StickyObject::Initialize()
{
	float scale = GetScale().x;
	//lineRenderer->AddLine(centerPoint, centerPoint + Vector3(2 / scale, 0, 0), Color(1, 0, 0, 1));
	//lineRenderer->AddLine(centerPoint, centerPoint + Vector3(0, 2 / scale, 0), Color(0, 1, 0, 1));
	//lineRenderer->AddLine(centerPoint, centerPoint + Vector3(0, 0, 2 / scale), Color(0, 0, 1, 1));
	GameObject::Initialize();
}

bool StickyObject::Attach(GameObject* gameObject)
{
	if (parent) return false;
	std::cout << "Attaching" << "\n";
	std::cout << GetCenterPoint().x << " "
		<< GetCenterPoint().y << " "
		<< GetCenterPoint().z << "\n";

	auto pWorld = gameObject->GetWorld().Invert();
	Quaternion pRotation;
	Vector3 pPos, pScale;
	pWorld.Decompose(pScale, pRotation, pPos);

	SetPosition(Vector3::Transform(GetPosition(), pWorld));
	SetRotation(GetRotation() * -pRotation);
	SetScale(GetScale().x * pScale.x);

	parent = gameObject;
	return true;
}

void StickyObject::SetCenterPoint(Vector3 newCenterPoint)
{
	centerPoint = Vector3::Transform(newCenterPoint, Matrix::CreateScale(GetScale()));
}

Vector3 StickyObject::GetCenterPoint()
{

	return Vector3::Transform(centerPoint, GetWorld());
}
