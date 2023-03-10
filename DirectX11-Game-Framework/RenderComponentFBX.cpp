#include "RenderComponentFBX.h"

#include <assimp/DefaultLogger.hpp>
#include "DirectXTex.h"

#include "Game.h"
#include "RenderSystem.h"
#include "Camera.h"


RenderComponentFBX::RenderComponentFBX(const std::string& shaderFileName, const std::string& modelFileName, const std::string& textureFileName) :
	modelFileName(modelFileName),
	textureFileName(textureFileName),
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
	const aiScene *scene = importer.ReadFile(modelFileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);

	// If the import failed, report it
	if (nullptr == scene) 
	{
		std::cout << "Missing file " << modelFileName << "\n";
		return;
	}

	SearchNode(scene, scene->mRootNode, aiMatrix4x4() * 0.01 * scale);

	Assimp::DefaultLogger::kill();

	if (textureFileName == "")
	{
		//std::cout << scene->mNumTextures << "\n";
		if (scene->HasTextures()) 
		{
			std::string folderPath = modelFileName;

			while (folderPath.length() > 0 && folderPath.back() != '\\' && folderPath.back() != '/')
			{
				folderPath.pop_back();
			}
			textureFileName = folderPath + scene->mTextures[0]->mFilename.C_Str();
		}
	}

	std::wstring fileName(textureFileName.begin(), textureFileName.end());
	DirectX::ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(fileName.c_str(), L".dds") == 0)
	{
		hr = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".tga") == 0)
	{
		hr = LoadFromTGAFile(fileName.c_str(), nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".hdr") == 0)
	{
		hr = LoadFromHDRFile(fileName.c_str(), nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	}

	RenderSystem* render = Game::GetRenderSystem();

	if (SUCCEEDED(hr))
	{
		hr = CreateShaderResourceView(render->Device.Get(),
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &textureView);
	} 
	else
	{
		std::cout << "Missing texture " << textureFileName << "\n";
	}

	RenderComponent::Initialize();

	/*std::cout << "\nVertices:\n";
	int i = 0;
	for (const auto& point : points) {
		i++;
		if (i % 2 == 0) continue;
		std::cout << "\n" << i / 2 << ": " << point.pos.x << " " << point.pos.y << " " << point.pos.z << "\n";
	}

	std::cout << "\nIndexes:\n";
	
	for (const auto& index : indexes) {
		
		std::cout << index << " ";
	}
	std::cout << "\n";*/

	//World = Matrix::Identity;
}

void RenderComponentFBX::Draw()
{
	RenderSystem* render = Game::GetRenderSystem();
	///Setup Rasterizer stage
	render->Context->RSSetState(rastState);

	///Setup AI stage
	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };

	render->Context->IASetInputLayout(layout);
	render->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	render->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);


	///Set Vertex and Pixel Resources
	render->Context->VSSetShader(vertexShader, nullptr, 0);
	render->Context->VSSetConstantBuffers(0, 1, &constBuffer);

	render->Context->PSSetShader(pixelShader, nullptr, 0);
	render->Context->PSSetShaderResources(0, 1, &textureView);
	render->Context->PSSetSamplers(0, 1, &samplerState);
	render->Context->PSSetConstantBuffers(0, 1, &constBuffer);
	render->Context->PSSetConstantBuffers(1, 1, &materialBuffer);

	render->Context->DrawIndexed(indexes.size(), 0, 0);
}

void RenderComponentFBX::Update()
{
	constBufferData.world = World;
	constBufferData.worldViewPosition = Game::GetCamera()->GetWorldViewPositionMatrix(World);
	auto camPos = Game::GetCamera()->position;
	constBufferData.cameraPos = Vector4(camPos.x, camPos.y, camPos.z, 1);
	UpdateConstBuffer();
}

void RenderComponentFBX::SearchNode(const aiScene* scene, aiNode* node, aiMatrix4x4 transform)
{
	transform *= node->mTransformation;

	if (node->mNumMeshes > 0) 
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			size_t nVertecis = mesh->mNumVertices;
			aiVector3D* vertecis = mesh->mVertices;
			size_t firstPointIndex = points.size();
			
			for (size_t i = 0; i < nVertecis; i++) 
			{
				aiVector3D vertex = vertecis[i];

				vertex *= transform;

				Vector3 point = Vector3(vertex.x,
					vertex.y,
					vertex.z) + offset;

				Vector2 UV = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

				Vector3 normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].z, -mesh->mNormals[i].y);

				points.push_back(VertexData{point, UV, normal});
			}


			size_t nFaces = mesh->mNumFaces;
			aiFace* meshFaces = mesh->mFaces;
			for (size_t i = 0; i < nFaces; i++) 
			{
				indexes.push_back(meshFaces[i].mIndices[0] + firstPointIndex);
				indexes.push_back(meshFaces[i].mIndices[1] + firstPointIndex);
				indexes.push_back(meshFaces[i].mIndices[2] + firstPointIndex);
			}

			meshes.push_back(mesh);
		}
	}

	// continue for all child nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		SearchNode(scene, node->mChildren[i], transform);
	}
}

