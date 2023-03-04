#include "RenderComponentFBX.h"

#include <assimp/DefaultLogger.hpp>

#include "Game.h"
#include "RenderSystem.h"
#include "Camera.h"


RenderComponentFBX::RenderComponentFBX(const std::string& shaderFileName, const std::string& fileName) :
	fileName(fileName),
	RenderComponent(shaderFileName)
{

}

RenderComponentFBX::~RenderComponentFBX()
{
}

void RenderComponentFBX::Initialize()
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene *scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate //|
		//aiProcess_JoinIdenticalVertices |
		//aiProcess_SortByPType
	);

	// If the import failed, report it
	if (nullptr == scene) 
	{
		std::cout << "Missing file " << fileName << "\n";
		return;
	}

	SearchNode(scene, scene->mRootNode, Matrix::Identity);

	Assimp::DefaultLogger::kill();

	RenderComponent::Initialize();

	std::cout << "\nVertices:\n";
	int i = 0;
	for (const auto& point : points) {
		i++;
		if (i % 2 == 0) continue;
		std::cout << "\n" << i / 2 << ": " << point.x << " " << point.y << " " << point.z << " " << point.w << "\n";
	}

	std::cout << "\nIndexes:\n";
	
	for (const auto& index : indexes) {
		
		std::cout << index << " ";
	}
	std::cout << "\n";

	//World = Matrix::Identity;
}

void RenderComponentFBX::Draw()
{
	RenderSystem* render = Game::GetRenderSystem();
	///Setup Rasterizer stage
	render->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	render->Context->IASetInputLayout(layout);
	render->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	render->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);


	///Set Vertex and Pixel Shaders
	render->Context->VSSetShader(vertexShader, nullptr, 0);
	render->Context->VSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetShader(pixelShader, nullptr, 0);

	render->Context->DrawIndexed(indexes.size(), 0, 0);
}

void RenderComponentFBX::Update()
{
	constBufferData.worldViewPosition = Game::GetCamera()->GetWorldViewPositionMatrix(World);
	UpdateConstBuffer();
}

void RenderComponentFBX::SearchNode(const aiScene* scene, aiNode* node, Matrix transform)
{
	if (node->mNumMeshes > 0) 
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			size_t nVertecis = mesh->mNumVertices;
			aiVector3D* vertecis = mesh->mVertices;

			for (size_t i = 0; i < nVertecis; i++) 
			{
				Vector4 point = Vector4(vertecis[i].x,
					vertecis[i].y,
					vertecis[i].z,
					1.0f);

				points.push_back(point);
				points.push_back(point);
				//points.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}

			size_t nFaces = mesh->mNumFaces;
			aiFace* meshFaces = mesh->mFaces;
			for (size_t i = 0; i < nFaces; i++) 
			{
				indexes.push_back(meshFaces[i].mIndices[0]); 
				indexes.push_back(meshFaces[i].mIndices[1]); 
				indexes.push_back(meshFaces[i].mIndices[2]);
			}
		}
	}

	// continue for all child nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		SearchNode(scene, node->mChildren[i], transform);
	}
}

