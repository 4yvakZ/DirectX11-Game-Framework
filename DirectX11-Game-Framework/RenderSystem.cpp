

#include "RenderSystem.h"

#include "DisplayWin.h"
#include "RenderComponent.h"
#include "ShadowMap.h"
#include "DirectionalLight.h"
#include "RenderComponentFBX.h"
#include "GBuffer.h"


//constexpr Color backgroundColor(0.2f, 0.2f, 0.2f);
constexpr Color backgroundColor(0.f, 0.f, 0.f);

void RenderSystem::CreateBackBuffer()
{
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);	// __uuidof(ID3D11Texture2D)
	Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);
}

void RenderSystem::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC stateDesc = {};
	stateDesc.DepthEnable = false;

	Device->CreateDepthStencilState(&stateDesc, &depthStencilStateOff);

	stateDesc.StencilEnable = true;

	Device->CreateDepthStencilState(&stateDesc, &depthStencilStateOn);
}

void RenderSystem::CreateLight(Vector4 lightDir)
{
	lightDir.Normalize();
	auto light = new DirectionalLight(Device, Context);
	light->lightData.direction = lightDir;
	light->Init();
	dirLights.push_back(light);
	shadowMaps.push_back(new ShadowMap(Device, Context, 2048, lightDir));
}

void RenderSystem::CreateLightShader()
{
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/LightRenderingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&lightVertexShaderByteCode,
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
			std::cout << "Missing Shader File: " << "../Shaders/LightRenderingShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		lightVertexShaderByteCode->GetBufferPointer(),
		lightVertexShaderByteCode->GetBufferSize(),
		nullptr, &lightVertexShader);

	res = D3DCompileFromFile(L"../Shaders/LightRenderingShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&lightPixelShaderByteCode,
		&errorVertexCode);

	Device->CreatePixelShader(
		lightPixelShaderByteCode->GetBufferPointer(),
		lightPixelShaderByteCode->GetBufferSize(),
		nullptr, &lightPixelShader);
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

	CreateDepthStencilStates();

	viewport = Viewport(0.0f, 0.0f, display->ClientWidth, display->ClientHeight);

	CreateLightShader();

	//CreateLight(Vector4(-1.0f, -4.0f, -1.0f, 0.0f));
	CreateLight(Vector4(1.0f, -2.0f, 1.0f, 0.0f));

	gBuffer = new GBuffer(Device, Context, viewport);
	gBuffer->Init();

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	Device->CreateRasterizerState(&rastDesc, &rastState);
}

RenderSystem::~RenderSystem()
{
	for (auto light : dirLights) {
		delete light;
	}
	dirLights.clear();

	for (auto shadowMap : shadowMaps) {
		delete shadowMap;
	}
	shadowMaps.clear();

	rastState->Release();
	delete gBuffer;
	RenderView->Release();
	backBuffer->Release();
	//DepthView->Release();
	//depthBuffer->Release();
	SwapChain->Release();
	Context->Release();
}

void RenderSystem::PrepareFrame()
{
	Context->ClearRenderTargetView(RenderView, backgroundColor);
	//Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	gBuffer->PrepareFrame();

	for (auto& shadowMap : shadowMaps) {
		shadowMap->PrepareFrame(Context);
	}
}


void RenderSystem::Draw()
{

	for (auto& shadowMap : shadowMaps) {

		shadowMap->Render(Context);

		for (auto& renderComponent : renderComponents) {
			if (auto fbx = dynamic_cast<RenderComponentFBX*>(renderComponent))
			{
				fbx->DrawShadows();
			}
		}
	}

	Context->GSSetShader(nullptr, nullptr, 0);
	gBuffer->Render();
	for (auto& renderComponent : renderComponents) {
		if (auto fbx = dynamic_cast<RenderComponentFBX*>(renderComponent))
		{
			fbx->DrawGeometry();
		}
	}

	ID3D11RenderTargetView* nullrtv[] = { nullptr,nullptr,nullptr,nullptr, nullptr,nullptr,nullptr,nullptr };
	Context->OMSetRenderTargets(8, nullrtv, nullptr);
	Context->RSSetViewports(1, viewport.Get11());
	Context->OMSetRenderTargets(1, &RenderView, nullptr);

	Context->OMSetDepthStencilState(depthStencilStateOff, 0);

	Context->VSSetShader(lightVertexShader, nullptr, 0);
	Context->PSSetShader(lightPixelShader, nullptr, 0);

	gBuffer->Bind();


	Context->RSSetState(rastState);
	Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	for (size_t i = 0; i < dirLights.size() && i < shadowMaps.size(); i++) {
		shadowMaps[i]->Bind(Context);
		dirLights[i]->Render();
		
		Context->Draw(4, 0);
	}
	Context->OMSetDepthStencilState(depthStencilStateOn, 0);

	/*for (auto& renderComponent : renderComponents) {
			renderComponent->Draw();
		}*/
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