#include "ShadowMap.h"


ShadowMap::ShadowMap(Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context, int width, Vector4 lightDir)
{
	Vector3 eye = -Vector3(lightDir.x, lightDir.y, lightDir.z);
	eye.Normalize();
	eye *= width / 100;
	Vector3 target = eye + lightDir;

	Matrix view = Matrix::CreateLookAt(eye, target, Vector3::Up);
	Matrix projection = Matrix::CreateOrthographic(

		width / 100,
		width / 100,
		0.1,
		1000);

	cascadeData.viewProjection = view * projection;

	auto coners = GetFrustrumCornersWorldSpace(view * projection);



	///const buffer initialization
	D3D11_BUFFER_DESC cascadeBufDesc = {};
	cascadeBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	cascadeBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cascadeBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cascadeBufDesc.MiscFlags = 0;
	cascadeBufDesc.StructureByteStride = 0;
	cascadeBufDesc.ByteWidth = sizeof(CascadeData);

	D3D11_SUBRESOURCE_DATA cascadeBufData = {};
	cascadeBufData.pSysMem = &cascadeData;
	cascadeBufData.SysMemPitch = 0;
	cascadeBufData.SysMemSlicePitch = 0;

	Device->CreateBuffer(&cascadeBufDesc, &cascadeBufData, &cascadeBuffer);
	
	Context->PSSetConstantBuffers(3, 1, &cascadeBuffer);
	Context->VSSetConstantBuffers(3, 1, &cascadeBuffer);

	CreateShaderResources(width, Device);
}

void ShadowMap::CreateShaderResources(int width, Microsoft::WRL::ComPtr<ID3D11Device>& Device)
{
	D3D11_TEXTURE2D_DESC shadowMapDesc = {};
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.Width = width;
	shadowMapDesc.Height = width;
	shadowMapDesc.SampleDesc = { 1, 0 };

	Device->CreateTexture2D(&shadowMapDesc, nullptr, &shadowMap);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	Device->CreateShaderResourceView(shadowMap, &shaderResourceViewDesc, &ShadowView);
	Device->CreateDepthStencilView(shadowMap, &depthStencilViewDesc, &DepthView);

	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	Device->CreateSamplerState(&comparisonSamplerDesc, &samplerState);

	viewport = Viewport(0.0f, 0.0f, width, width);

	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"../Shaders/SimpleShadowMapShader.hlsl",
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
			std::cout << "Missing Shader File: " << "../Shaders/SimpleShadowMapShader.hlsl" << std::endl;
		}

		return;
	}

	Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	//rastDesc.DepthBias = 0.01;
	//rastDesc.DepthBiasClamp = 0.05;
	//rastDesc.SlopeScaledDepthBias = 1;

	Device->CreateRasterizerState(&rastDesc, &rastState);
}

std::vector<Vector4> ShadowMap::GetFrustrumCornersWorldSpace(const Matrix& viewProjection)
{
	const auto inv = viewProjection.Invert();

	std::vector<Vector4> frustrumCorners;
	frustrumCorners.reserve(8);
	for (size_t x = 0; x < 2; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			for (size_t z = 0; z < 2; z++)
			{
				const Vector4 pt = Vector4::Transform(
					Vector4(2.0f*x -1.0f,
						2.0f * y - 1.0f, 
						2.0f * z - 1.0f, 
						1.0f), inv);
			}
		}
	}

	return frustrumCorners;
}

ShadowMap::~ShadowMap()
{
	DepthView->Release();

	shadowMap->Release();
	ShadowView->Release();

	samplerState->Release();
}

void ShadowMap::Render(ID3D11DeviceContext* Context)
{
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	Context->PSSetShaderResources(1, 1, nullsrv);

	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Context->OMSetRenderTargets(0, nullptr, DepthView);
	
	Context->RSSetState(rastState);

	Context->RSSetViewports(1, viewport.Get11());
	Context->VSSetShader(vertexShader, nullptr, 0);
	Context->PSSetShader(nullptr, nullptr, 0);
}

void ShadowMap::Bind(ID3D11DeviceContext* Context)
{
	Context->PSSetShaderResources(1, 1, &ShadowView);
	Context->PSSetSamplers(1, 1, &samplerState);
}
