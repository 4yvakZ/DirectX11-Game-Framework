#pragma once
#include "RenderComponent.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags  

class GAMEFRAMEWORK_API RenderComponentFBX :
	public RenderComponent
{
public:
	RenderComponentFBX(const std::string& shaderFileName, const std::string& modelFileName, const std::string& textureFileName = "");

	RenderComponentFBX() = delete;

	~RenderComponentFBX();

	// Inherited via RenderComponent
	virtual void Initialize() override;
	virtual void Draw() override;
	virtual void Update() override;

private:
	void SearchNode(const aiScene *scene, aiNode *node, Matrix Transform);

	const std::string modelFileName;
	const std::string textureFileName;

	std::vector<aiMesh*> meshes;
};

