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
	void DrawShadows();
	virtual void Update() override;

private:
	void SearchNode(const aiScene *scene, aiNode *node, aiMatrix4x4 Transform);

public:
	Vector3 offset = Vector3(0, 0, 0);
	float scale = 1;
	bool isZUp = true;

private:
	const std::string modelFileName;
	std::string textureFileName;

	std::vector<aiMesh*> meshes;
};

