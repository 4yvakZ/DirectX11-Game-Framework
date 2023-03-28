#pragma pack_matrix( row_major )

Texture2D DiffuseMap : register(t0);

SamplerState Sampler : register(s0);


cbuffer OBJECT_CONST_BUF : register(b0)
{
    Matrix view;
    Matrix projection;
    Matrix world;
    float4 cameraPos;
}

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION;
    float3 normal : NORMAL0;
};

struct MaterialData
{
    float4 ambient;
    float4 diffuse;
    float4 specularAlpha;
};

cbuffer MATERIAL_CONST_BUF : register(b1)
{
    MaterialData material;
}

struct GBuffer
{
    float4 worldPos : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 specular : SV_Target3;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    float4 pos = mul(float4(input.pos.xyz, 1.0f), world);
    output.worldPos = pos;
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;
    
    output.uv = input.uv;
    output.normal = mul(float4(input.normal, 0.0f), world);

	return output;
}


GBuffer PSMain(PS_IN input)
{
    GBuffer output = (GBuffer) 0;
    
    output.worldPos = input.worldPos;
    output.normal = float4(input.normal, 0.0f);
    output.albedo = DiffuseMap.Sample(Sampler, input.uv);
    output.specular = material.specularAlpha;
    
    return output;
}