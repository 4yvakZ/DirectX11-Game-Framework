#pragma pack_matrix( row_major )

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

cbuffer WORLD_CONST_BUF : register(b0)
{
    Matrix worldViewPos;
    Matrix world;
}

cbuffer MATERIAL_CONST_BUF : register(b1)
{
    float4 ambient;
    float4 diffuse;
    float4 specularAlpha;
}

cbuffer LIGHT_CONST_BUF : register(b2)
{
    float4 direction;
    float4 intensity;
}

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
    
    output.pos = mul(float4(input.pos.xyz, 1.0f), worldViewPos);
    output.color = input.color;
    output.uv = input.uv;
    output.normal = mul(float4(input.normal.xyz, 0.0f), world);
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 color;
    
    input.normal = normalize(input.normal);

#if TEXTURE
    color = DiffuseMap.Sample(Sampler, input.uv.xy);
#else
    color = input.color;
#endif
    //float3 diffuse = ;
    float4 ambientColor = color * float4(ambient.xyz, 1.0f);
    //float3 speculat = ;
    
    color = ambientColor;
    
    return color;
}