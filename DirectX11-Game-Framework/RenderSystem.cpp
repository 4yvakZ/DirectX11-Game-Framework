

#include "RenderSystem.h"

#include "DisplayWin.h"
#include "RenderComponent.h"
#include "ShadowMap.h"
#include "RenderComponentFBX.h"


//constexpr Color backgroundColor(0.2f, 0.2f, 0.2f);
constexpr Color backgroundColor(0.f, 0.f, 0.f);

void RenderSystem::CreateBackBuffer()
{
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);	// __uuidof(ID3D11Texture2D)
	Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);
}

void RenderSystem::CreateDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = display->ClientWidth;
	depthTexDesc.Height = display->ClientHeight;
	depthTexDesc.SampleDesc = { 1, 0 };


	Device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);
	Device->CreateDepthStencilView(depthBuffer, nullptr, &DepthView);
}

void RenderSystem::CreateLightBuffer()
{
	lightData.direction = Vector4(-1, -4, -1, 0);
	lightData.direction.Normalize();
	Vector3 eye = -Vector3(lightData.direction.x, lightData.direction.y, lightData.direction.z);
	eye.Normalize();
	eye *= 1024 /100;
	Vector3 target = eye + lightData.direction;
	
	lightData.view = Matrix::CreateLookAt(eye, target, Vector3::Up);
	lightData.projection = Matrix::CreateOrthographic(

		1024 / 100,
		1024 /100,
		0.1,
		1000);

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
	Context->PSSetConstantBuffers(2, 1, &lightBuffer);
	Context->VSSetConstantBuffers(2, 1, &lightBuffer);
}

void RenderSystem::CreateShadowMap()
{
	shadowMap = new ShadowMap(Device, 1024);
}

RenderSystem::RenderSystem(DisplayWin *display):
	display(display)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = display->ClientWidth;
	swapDesc.BufferDesc.Height = display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr, //видеокарта (поменять на дискретную на ноуте?)
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	CreateBackBuffer();

	CreateShadowMap();

	CreateDepthBuffer();

	viewport = Viewport(0.0f, 0.0f, display->ClientWidth, display->ClientHeight);

	CreateLightBuffer();

}

RenderSystem::~RenderSystem()
{
	delete shadowMap;
	lightBuffer->Release();
	RenderView->Release();
	backBuffer->Release();
	DepthView->Release();
	depthBuffer->Release();
	SwapChain->Release();
	Context->Release();
}

void RenderSystem::PrepareFrame()
{
	Context->ClearRenderTargetView(RenderView, backgroundColor);	
	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	shadowMap->Render(Context);
	for (auto& renderComponent : renderComponents) {
		if (auto fbx = dynamic_cast<RenderComponentFBX*>(renderComponent))
		{
			fbx->DrawShadows();
		}
	}

	Context->RSSetViewports(1, viewport.Get11());
	Context->OMSetRenderTargets(1, &RenderView, DepthView);

	shadowMap->Bind(Context);
}


void RenderSystem::Draw()
{
	for (auto& renderComponent : renderComponents) {
		renderComponent->Draw();
	}
}

void RenderSystem::EndFrame()
{
	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}
void RenderSystem::RemoveRenderComponent(RenderComponent* renderComponent)
{
	for (size_t i = 0; i < renderComponents.size(); i++) {
		if (renderComponents[i] == renderComponent) {
			renderComponents.erase(renderComponents.begin() + i);
			return;
		}
	}
}

void RenderSystem::UpdateLightData(LightData newLightData)
{
	lightData = newLightData;
	lightData.direction.Normalize();

	Context->UpdateSubresource(lightBuffer, 0, nullptr, &lightData, 0, 0);
}
