#pragma pack_matrix( row_major )


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
    float4 worldPos : POSITION;
};

struct GBuffer
{
    float4 worldPos : SV_Target0;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    float4 pos = mul(float4(input.pos.xyz, 1.0f), world);
    output.worldPos = pos;
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;

	return output;
}


GBuffer PSMain(PS_IN input)
{
    GBuffer output = (GBuffer) 0;
    //output.worldPos = float4(1, 1, 1, 1);
    output.worldPos = input.worldPos;
    return output;
}