#pragma once
#include "framework.h"

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

#pragma pack(push,4)
	struct Emitter {
		float minLifetima;
		float maxLifetime;
		float minSpawnVelocity;
		float maxSpawnVelocity;
		float minSize;
		float maxSize;
		float deltaTime;
		float blank;
		Vector4 force;
		Vector4 minSpawnPos;
		Vector4 maxSpawnPos;
	};
#pragma pack(pop)

	struct Particle
	{
		Vector4 position;
		Vector4 velocity;
		Vector4 acceleration;
		float timeUntilDeath;
		float size;
	};

public:
	ParticleSystem() = delete;
	ParticleSystem(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context):
		Device(Device),
		Context(Context)
	{}

	~ParticleSystem();

	void Initialize();
public:
	int maxNumberOfParticles = 65536;
	int maxSpawnRate = 100;
	int nAliveParticles = 0;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;

	ID3D11ShaderResourceView* textureView = nullptr;
	ID3D11SamplerState* samplerState;
};

