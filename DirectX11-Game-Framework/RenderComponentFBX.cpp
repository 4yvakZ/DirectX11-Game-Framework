#include "RenderComponentFBX.h"
#include  "FBXImporter.h"

RenderComponentFBX::RenderComponentFBX(std::string shaderFileName, const char* fileName) :
	fileName(fileName),
	RenderComponent(shaderFileName)
{
	
}

void RenderComponentFBX::Initialize()
{
	FBXImporter importer;
	importer.LoadFBXFile(fileName);
	std::cout << importer.GetMeshesCount() << "\n";
	


	RenderComponent::Initialize();
}

void RenderComponentFBX::Draw()
{
}

void RenderComponentFBX::Update()
{
}
