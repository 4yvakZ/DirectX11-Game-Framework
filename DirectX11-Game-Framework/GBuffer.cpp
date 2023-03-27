#include "GBuffer.h"

void GBuffer::CreateDepthBuffer() {
	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = viewport.width;
	depthTexDesc.Height = viewport.height;
	depthTexDesc.SampleDesc = { 1, 0 };

	auto res = Device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);
	res = Device->CreateDepthStencilView(depthBuffer, nullptr, &depthView);
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

GBuffer::~GBuffer()
{
	vertexShaderByteCode->Release();
	vertexShader->Release();

	pixelShaderByteCode->Release();
	pixelShader->Release();

	depthBuffer->Release();
	depthView->Release();

	positionRTV->Release();
	positionSRV->Release();
	positionTexture->Release();
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

	CreateDepthBuffer();
	CreatePositionResources();
}

void GBuffer::Render()
{
	Context->ClearRenderTargetView(positionRTV, Color(0, 0, 0, 0));
	Context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	Context->RSSetViewports(1, viewport.Get11());
	
	ID3D11RenderTargetView* renderTargets[] = {positionRTV};
	Context->OMSetRenderTargets(1, renderTargets, depthView);

	Context->VSSetShader(vertexShader, nullptr, 0);
	Context->PSSetShader(pixelShader, nullptr, 0);
}
