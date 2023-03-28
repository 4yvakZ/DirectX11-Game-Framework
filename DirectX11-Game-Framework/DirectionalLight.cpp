#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context):
	Device(Device),
	Context(Context)
{
}

DirectionalLight::~DirectionalLight()
{
	lightBuffer->Release();
}

void DirectionalLight::CreateLightBuffer()
{
	lightData.direction.Normalize();

	///const buffer initialization
	D3D11_BUFFER_DESC lightBufDesc = {};
	lightBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufDesc.MiscFlags = 0;
	lightBufDesc.StructureByteStride = 0;
	lightBufDesc.ByteWidth = sizeof(LightData);

	D3D11_SUBRESOURCE_DATA lightBufData = {};
	lightBufData.pSysMem = &lightData;
	lightBufData.SysMemPitch = 0;
	lightBufData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&lightBufDesc, &lightBufData, &lightBuffer);
}

void DirectionalLight::Init()
{
	CreateLightBuffer();
}

void DirectionalLight::Render()
{
	Context->PSSetConstantBuffers(2, 1, &lightBuffer);
	Context->VSSetConstantBuffers(2, 1, &lightBuffer);
}
