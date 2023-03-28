#include "GBuffer.h"

void GBuffer::CreateDepthResouces() {
	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = viewport.width;
	depthTexDesc.Height = viewport.height;
	depthTexDesc.SampleDesc = { 1, 0 };

	auto res = Device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	res = Device->CreateShaderResourceView(depthBuffer, &shaderResourceViewDesc, &depthSRV);
	res = Device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthView);
}

void GBuffer::CreatePositionResources() {

	D3D11_TEXTURE2D_DESC positionTexDesc = {};
	positionTexDesc.ArraySize = 1;
	positionTexDesc.MipLevels = 1;
	positionTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positionTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	positionTexDesc.CPUAccessFlags = 0;
	positionTexDesc.Usage = D3D11_USAGE_DEFAULT;
	positionTexDesc.Height = viewport.height;
	positionTexDesc.Width = viewport.width;
	positionTexDesc.SampleDesc = { 1,0 };

	auto res = Device->CreateTexture2D(&positionTexDesc, nullptr, &positionTexture);

	D3D11_RENDER_TARGET_VIEW_DESC positionRTVDesc = {};
	positionRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positionRTVDesc.Texture2D.MipSlice = 0;
	positionRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	res = Device->CreateRenderTargetView(positionTexture, &positionRTVDesc, &positionRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC positionSRVDesc = {};
	positionSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positionSRVDesc.Texture2D.MipLevels = 1;
	positionSRVDesc.Texture2D.MostDetailedMip = 0;
	positionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	res = Device->CreateShaderResourceView(positionTexture, &positionSRVDesc, &positionSRV);
}

void GBuffer::CreateNormalResouces()
{
	D3D11_TEXTURE2D_DESC normalTexDesc = {};
	normalTexDesc.ArraySize = 1;
	normalTexDesc.MipLevels = 1;
	normalTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	normalTexDesc.CPUAccessFlags = 0;
	normalTexDesc.Usage = D3D11_USAGE_DEFAULT;
	normalTexDesc.Height = viewport.height;
	normalTexDesc.Width = viewport.width;
	normalTexDesc.SampleDesc = { 1,0 };

	auto res = Device->CreateTexture2D(&normalTexDesc, nullptr, &normalTexture);

	D3D11_RENDER_TARGET_VIEW_DESC normalRTVDesc = {};
	normalRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalRTVDesc.Texture2D.MipSlice = 0;
	normalRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	res = Device->CreateRenderTargetView(normalTexture, &normalRTVDesc, &normalRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC normalSRVDesc = {};
	normalSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalSRVDesc.Texture2D.MipLevels = 1;
	normalSRVDesc.Texture2D.MostDetailedMip = 0;
	normalSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	res = Device->CreateShaderResourceView(normalTexture, &normalSRVDesc, &normalSRV);
}

void GBuffer::CreateAlbedoResouces()
{
	D3D11_TEXTURE2D_DESC albedoTexDesc = {};
	albedoTexDesc.ArraySize = 1;
	albedoTexDesc.MipLevels = 1;
	albedoTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	albedoTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	albedoTexDesc.CPUAccessFlags = 0;
	albedoTexDesc.Usage = D3D11_USAGE_DEFAULT;
	albedoTexDesc.Height = viewport.height;
	albedoTexDesc.Width = viewport.width;
	albedoTexDesc.SampleDesc = { 1,0 };

	auto res = Device->CreateTexture2D(&albedoTexDesc, nullptr, &albedoTexture);

	D3D11_RENDER_TARGET_VIEW_DESC albedoRTVDesc = {};
	albedoRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	albedoRTVDesc.Texture2D.MipSlice = 0;
	albedoRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	res = Device->CreateRenderTargetView(albedoTexture, &albedoRTVDesc, &albedoRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC albedoSRVDesc = {};
	albedoSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	albedoSRVDesc.Texture2D.MipLevels = 1;
	albedoSRVDesc.Texture2D.MostDetailedMip = 0;
	albedoSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	res = Device->CreateShaderResourceView(albedoTexture, &albedoSRVDesc, &albedoSRV);
}

void GBuffer::CreateSpecularResources()
{
	D3D11_TEXTURE2D_DESC specularTexDesc = {};
	specularTexDesc.ArraySize = 1;
	specularTexDesc.MipLevels = 1;
	specularTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specularTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	specularTexDesc.CPUAccessFlags = 0;
	specularTexDesc.Usage = D3D11_USAGE_DEFAULT;
	specularTexDesc.Height = viewport.height;
	specularTexDesc.Width = viewport.width;
	specularTexDesc.SampleDesc = { 1,0 };

	auto res = Device->CreateTexture2D(&specularTexDesc, nullptr, &specularTexture);

	D3D11_RENDER_TARGET_VIEW_DESC specularRTVDesc = {};
	specularRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specularRTVDesc.Texture2D.MipSlice = 0;
	specularRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	res = Device->CreateRenderTargetView(specularTexture, &specularRTVDesc, &specularRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC specularSRVDesc = {};
	specularSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specularSRVDesc.Texture2D.MipLevels = 1;
	specularSRVDesc.Texture2D.MostDetailedMip = 0;
	specularSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	res = Device->CreateShaderResourceView(specularTexture, &specularSRVDesc, &specularSRV);
}

GBuffer::~GBuffer()
{
	vertexShaderByteCode->Release();
	vertexShader->Release();

	pixelShaderByteCode->Release();
	pixelShader->Release();

	depthBuffer->Release();
	depthSRV->Release();
	depthView->Release();

	positionRTV->Release();
	positionSRV->Release();
	positionTexture->Release();

	normalRTV->Release();
	normalSRV->Release();
	normalTexture->Release();

	albedoRTV->Release();
	albedoSRV->Release();
	albedoTexture->Release();

	specularRTV->Release();
	specularSRV->Release();
	specularTexture->Release();
}

void GBuffer::Init()
{

	///TODO Move to RenderComponent

	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/GeometryRenderingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << "../Shaders/GeometryRenderingShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	res = D3DCompileFromFile(L"../Shaders/GeometryRenderingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorVertexCode);

	Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	///---------

	CreateDepthResouces();
	CreatePositionResources();
	CreateNormalResouces();
	CreateAlbedoResouces();
	CreateSpecularResources();
}

void GBuffer::PrepareFrame() {
	Context->ClearRenderTargetView(positionRTV, Color(0, 0, 0, 0));
	Context->ClearRenderTargetView(normalRTV, Color(0, 0, 0, 0));
	Context->ClearRenderTargetView(albedoRTV, Color(0, 0, 0, 0));
	Context->ClearRenderTargetView(specularRTV, Color(0, 0, 0, 0));
	Context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GBuffer::Render()
{
	Context->RSSetViewports(1, viewport.Get11());

	ID3D11ShaderResourceView* nullsrv[] = { nullptr, nullptr, nullptr, nullptr };
	Context->PSSetShaderResources(2, 4, nullsrv);
	
	ID3D11RenderTargetView* renderTargets[] = {positionRTV, normalRTV, albedoRTV, specularRTV};
	Context->OMSetRenderTargets(4, renderTargets, depthView);

	Context->VSSetShader(vertexShader, nullptr, 0);
	Context->PSSetShader(pixelShader, nullptr, 0);
}

void GBuffer::Bind()
{
	ID3D11ShaderResourceView* nullsrv[] = { positionSRV, normalSRV, albedoSRV, specularSRV };
	Context->PSSetShaderResources(2, 4, nullsrv);
}
