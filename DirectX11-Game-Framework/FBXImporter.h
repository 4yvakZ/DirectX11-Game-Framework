#pragma once
#include <fbxsdk.h>

#include "framework.h"

using namespace DirectX::SimpleMath;

class FBXImporter
{
public: 
	
	void LoadFBXFile(const char* file);

	FbxMesh* operator[](size_t index) {
		return meshes[index];
	}

	size_t GetMeshesCount() {
		return meshes.size();
	}

	void GetMeshData(size_t meshIndex, std::vector<Vector4>& points, std::vector<int>& indexes);

private:
	
	void Initialize();
	void GetScene(FbxNode* root = nullptr);
	void GetMesh(FbxMesh* mesh);
	

public:
	FBXImporter() {
		Initialize();
	}

	~FBXImporter() {
		for (const auto& mesh : meshes) {
			mesh->Destroy();
		}
		meshes.clear();
		scene->Destroy();
		manager->Destroy();
	}

private:
	static FBXImporter* instance;
	FbxManager* manager;
	FbxScene* scene;

	std::vector<FbxMesh*> meshes;
};

