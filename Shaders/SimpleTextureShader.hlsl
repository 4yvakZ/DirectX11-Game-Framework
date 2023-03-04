#pragma pack_matrix( row_major )

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer VS_CONST_BUF : register(b0)
{
    Matrix worldViewPos;
}

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
    
    output.pos = mul(float4(input.pos.xyz, 1.0f), worldViewPos);
    output.color = input.color;
    output.uv = input.uv;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 color;

#if TEXTURE
    color = DiffuseMap.Sample(Sampler, input.uv.xy);
#else
    color = input.color;
#endif
    
    return color;
}