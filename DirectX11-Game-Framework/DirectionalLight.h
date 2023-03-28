#pragma once

#include "framework.h"

using namespace DirectX::SimpleMath;

class GAMEFRAMEWORK_API  DirectionalLight
{
public:
	DirectionalLight(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context);
	DirectionalLight() = delete;
	~DirectionalLight();



	struct LightData
	{
		Vector4 direction{ -1.0f, -4.0f, -1.0f, 0.0f };
		Vector4 intensity{ 1.0f, 1.0f, 1.0f, 0.0f };
	};

	void Init();

	void Render();

private:
	void CreateLightBuffer();
public:
	LightData lightData;
	ID3D11Buffer* lightBuffer;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
};

