#pragma pack_matrix( row_major )



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
};

cbuffer OBJECT_CONST_BUF : register(b0)
{
    Matrix worldViewPos;
    Matrix world;
    float4 cameraPos;
}

struct LightData
{
    float4 direction;
    float4 intensity;
    Matrix view;
    Matrix projection;
};

cbuffer LIGHT_CONST_BUF : register(b2)
{
    LightData light;
}

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
    
    float4 pos = mul(float4(input.pos.xyz, 1.0f), world);
    pos = mul(pos, light.view);
    output.pos = mul(pos, light.projection);

	return output;
}