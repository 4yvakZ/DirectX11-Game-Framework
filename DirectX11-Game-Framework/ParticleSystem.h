#pragma once
#include "framework.h"

constexpr UINT THREAD_GROUP_X = 32;
constexpr UINT THREAD_GROUP_Y = 32;
constexpr UINT THREAD_GROUP_TOTAL = 1024;

constexpr UINT BITONIC_BLOCK_SIZE = 512;
constexpr UINT TRANSPOSE_BLOCK_SIZE = 16;
constexpr UINT MATRIX_WIDTH = BITONIC_BLOCK_SIZE;


using namespace DirectX::SimpleMath;

class ParticleSystem
{
	struct ConstBufferData
	{
		Matrix view;
		Matrix projection;
		Matrix world;
		Vector4 cameraPos;
	}; 
	struct SortingCB
	{
		UINT iLevel;
		UINT iLevelMask;
		UINT iWidth;
		UINT iHeight;
	};

#pragma pack(push,4)
	struct Emitter {
		float minLifetima = 10;
		float maxLifetime = 10;
		float minSpawnVelocity = 0;
		float maxSpawnVelocity = 0;

		float minSize = 0.05;
		float maxSize = 0.1;
		float deltaTime;
		float blank0;

		UINT emitCount;
		UINT maxNumberOfParticles; 
		UINT blank1;
		UINT blank2;

		Vector4 force = Vector4(0, -0.04, 0, 0);
		Vector4 minSpawnPos = Vector4(-0.5, 0.0, -0.5, 1);
		Vector4 maxSpawnPos = Vector4(0.5, 0.2, 0.5, 1);
	};
#pragma pack(pop)

	struct Particle
	{
		Vector4 position;
		Vector4 velocity;
		float timeUntilDeath;
		float size;
	};

	struct SortingData {
		UINT index;
		float depth;
	};


public:
	ParticleSystem() = delete;
	ParticleSystem(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context):
		Device(Device),
		Context(Context)
	{
		emitter.maxNumberOfParticles = maxNumberOfParticles;
	}

	~ParticleSystem();

	void Initialize();
	void Update(float deltaTime);
	void Emit(int nParticles);
	void Render();

protected:
	void InitResouces();
	void LoadTexture();
	void LoadShaders();
	void UpdateConstBuffer();
	void SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);
	void SortParticles();
public:
	UINT maxNumberOfParticles = 65536;
	int maxSpawnRate = 100;
	int nAliveParticles = 0;

	Matrix World;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;

	///Shader Resources
	ConstBufferData constBufferData;
	ID3D11Buffer* constBuffer;

	Emitter emitter;
	ID3D11Buffer* emitterBuffer;

	SortingCB sortingCB;
	ID3D11Buffer* sortingCBuffer;

	ID3D11ShaderResourceView* textureView = nullptr;
	ID3D11SamplerState* samplerState;

	ID3D11Buffer* particlesBuffer;
	ID3D11UnorderedAccessView* particlesUAV;
	ID3D11ShaderResourceView* particlesSRV;

	ID3D11Buffer* deadListBuffer;
	ID3D11UnorderedAccessView* deadListUAV;

	ID3D11Buffer* sortedListBuffer;
	ID3D11UnorderedAccessView* sortedListUAV;
	ID3D11ShaderResourceView* sortedListSRV;

	ID3D11Buffer* indirectBuffer;


	///Shaders
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;

	ID3D11GeometryShader* geometryShader;
	ID3DBlob* geometryShaderByteCode;

	ID3D11ComputeShader* sortShader;
	ID3DBlob* sortShaderByteCode;
	
	ID3D11ComputeShader* transposeShader;
	ID3DBlob* transposeShaderByteCode;

	ID3D11ComputeShader* emitShader;
	ID3DBlob* emitShaderByteCode;

	ID3D11ComputeShader* simulateShader;
	ID3DBlob* simulateShaderByteCode;
};

