#include "FBXImporter.h"

//#pragma comment (lib, "C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2020.0.1\\lib\\vs2017\\x64\\debug\\libfbxsdk-md.lib")

void FBXImporter::Initialize()
{
	manager = FbxManager::Create();
	assert(manager);

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);
}

void FBXImporter::LoadFBXFile(const char* file)
{
	assert(manager && !scene);

	for (const auto& mesh : meshes) {
		mesh->Destroy();
	}
	meshes.clear();

	scene = FbxScene::Create(manager, "Importer Scene");

	FbxImporter* importer(FbxImporter::Create(manager, "Importer"));

	bool res = importer->Initialize(file, -1, manager->GetIOSettings());

	if (!res) {
		std::cout << "Missing file " << file << std::endl;
		return;
	}

	res = importer->Import(scene);
	if (!res) {
		std::cout << "Failed to import file " << file << std::endl;
		return;
	}

	importer->Destroy();

	GetScene();
}

void FBXImporter::GetScene(FbxNode* root)
{
	assert(manager && scene);

	if (!root) {
		root = scene->GetRootNode();
		if (!root) {
			std::cout << "Empty Scene\n";
			return;
		}
	}

	const int nNodes = root->GetChildCount();

	for (int i = 0; i < nNodes; i++) {
		FbxNode* node{ root->GetChild(i) };
		
		if (!node) {
			continue;
		}

		FbxMesh* mesh = node->GetMesh();
		if (mesh) {
			GetMesh(mesh);
		}
		else {
			GetScene(node);
		}
	}
}

void FBXImporter::GetMesh(FbxMesh* mesh)
{
	assert(mesh);

	int res = mesh->RemoveBadPolygons();
	assert(res != -1);

	///Triangulation
	FbxGeometryConverter gc{ manager };

	mesh = static_cast<FbxMesh*>(gc.Triangulate(mesh, true));

	assert(mesh);

	res = mesh->RemoveBadPolygons();
	assert(res != -1);

	meshes.push_back(mesh);

	//TODO: Mesh to buffers?
}

void FBXImporter::GetMeshData(size_t meshIndex, std::vector<Vector4>& points, std::vector<int>& indexes)
{
	assert(meshIndex < meshes.size());
	FbxMesh* mesh = meshes[meshIndex];

	size_t firtsPointIndex = points.size() / 2;

	fbxsdk::FbxVector4* vertecis = mesh->GetControlPoints();
	size_t nVertecis = mesh->GetControlPointsCount();

	for (size_t i = 0; i < nVertecis; i++) {
		Vector4 point = Vector4(vertecis[i].mData[0],
			vertecis[i].mData[1],
			vertecis[i].mData[2],
			1.0f);

		points.push_back(point);

		points.push_back(point);
		//points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	size_t nIndexes = mesh->GetPolygonVertexCount();
	int* meshIndexes = mesh->GetPolygonVertices();

	for (size_t i = 0; i < nIndexes; i++) {
		indexes.push_back(meshIndexes[i]);
	}
}
