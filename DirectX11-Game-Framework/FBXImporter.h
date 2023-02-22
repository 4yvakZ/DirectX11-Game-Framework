#pragma once
#include <fbxsdk.h>

#include "framework.h"

class FBXImporter
{

	void Initialize();
	void LoadFBXFile(const char* file);

	void GetScene(FbxNode* root = nullptr);
	void GetMesh(FbxMesh* mesh);

private:
	FbxManager* manager;
	FbxScene* scene;
};

