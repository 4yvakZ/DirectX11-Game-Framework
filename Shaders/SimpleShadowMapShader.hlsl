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

struct GS_IN
{
    float4 pos : POSITION;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    uint arrInd : SV_RenderTargetArrayIndex;
};

struct CascadeData
{
    Matrix viewProjection[4];
    float4 distances;
};

cbuffer CASCADE_CONST_BUF : register(b3)
{
    CascadeData cascade;
}

GS_IN VSMain(VS_IN input)
{
    GS_IN output = (GS_IN) 0;
    
    output.pos = mul(float4(input.pos.xyz, 1.0f), world);

	return output;
}

[instance(4)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN p[3],
    in uint id : SV_GSInstanceID,
    inout TriangleStream<GS_OUT> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_OUT gs = (GS_OUT) 0;
        gs.pos = mul(float4(p[i].pos.xyz, 1.0f), cascade.viewProjection[id]);
        gs.arrInd = id;
        stream.Append(gs);
    }
}