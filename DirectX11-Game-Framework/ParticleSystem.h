#pragma once
#include "framework.h"

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
		float minLifetima = 2;
		float maxLifetime = 2;
		float minSpawnVelocity = 0;
		float maxSpawnVelocity = 0;

		float minSize = 0.01;
		float maxSize = 0.05;
		float deltaTime;
		float blank0;

		UINT emitCount;
		UINT maxNumberOfParticles; 
		UINT numGroups;
		UINT blank2;

		Vector4 force = Vector4(5, -9.81, 0, 0);
		Vector4 minSpawnPos = Vector4(-5, 0.0, -5, 1);
		Vector4 maxSpawnPos = Vector4(5, 0.2, 5, 1);
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

	struct HeightMap
	{
		Matrix viewProjection;
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
	void RenderHeightMap();
	void BindHeightMap();

protected:
	void InitResouces();
	void InitHeightMap(int width = 2048);
	void LoadTexture();
	void LoadShaders();
	void UpdateConstBuffer();
	void SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);
	void SortParticles();
	UINT GetDispatchDim(UINT nParticles);
public:
	UINT maxNumberOfParticles = 65536;
	int maxSpawnRate = 100;
	int nAliveParticles = 0;

	Matrix World;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;

	///HeightMap stuff
	ID3D11VertexShader* HMVertexShader;
	ID3DBlob* HMVertexShaderByteCode;


	ID3D11DepthStencilView* DepthView;
	ID3D11Texture2D* heightMap;
	ID3D11ShaderResourceView* HeightView;

	ID3D11SamplerState* HMSamplerState;
	HeightMap heightMapData;
	ID3D11Buffer* heightMapBuf;

	Viewport viewport;

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

