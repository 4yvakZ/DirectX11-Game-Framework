#pragma pack_matrix( row_major )

Texture2D Albedo : register(t4);
Texture2D<float3> Light : register(t6);

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};


PS_IN VSMain( uint id: SV_VertexID )
{
	PS_IN output = (PS_IN)0;
    
    output.tex = float2(id & 1, id >> 1);
    output.pos = float4(output.tex * float2(2, -2) + float2(-1, 1), 0, 1);
    
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target0
{
    uint width, height;
    Albedo.GetDimensions(width, height);
    uint2 pixelCoord = uint2(input.tex.x * width, input.tex.y * height);
    
    float4 albedo = Albedo.Load(uint3(pixelCoord, 0));
    float3 lighting = Light.Load(uint3(pixelCoord, 0));
    
    return pow(float4(lighting, 1) * albedo, 0.45);
}