#pragma pack_matrix( row_major )


Texture2DArray ShadowMap : register(t1);
Texture2D Position : register(t2);
Texture2D<float3> Normal : register(t3);
Texture2D Specular : register(t5);

SamplerComparisonState ShadowSampler : register(s1);

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer OBJECT_CONST_BUF : register(b0)
{
    Matrix view;
    Matrix projection;
    Matrix world;
    float4 cameraPos;
}

struct LightData
{
    float4 direction;
    float4 intensity;
};

cbuffer LIGHT_CONST_BUF : register(b2)
{
    LightData light;
}

struct CascadeData
{
    Matrix viewProjection[4];
    float4 distances;
};

cbuffer CASCADE_CONST_BUF : register(b3)
{
    CascadeData cascade;
}

int getCascadeLayer(float depthValue)
{
    int layer = 3;
    for (int i = 0; i < 4; i++)
    {
        if (depthValue < cascade.distances[i])
        {
            layer = i;
            break;
        }
    }
    return layer;
}

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
    Position.GetDimensions(width, height);
    uint2 pixelCoord = uint2(input.tex.x * width, input.tex.y * height);
    //return float4(1, 0, 0, 1);
    float4 worldPos =  Position.Load(uint3(pixelCoord, 0));
    if (worldPos.w == 0)
    {
        return float4(0, 0, 0, 1);
    }
    float3 normal = normalize(Normal.Load(uint3(pixelCoord, 0)));
    float4 specularAlpha = Specular.Load(uint3(pixelCoord, 0));
    
    int layer = getCascadeLayer(abs(mul(worldPos, view).z));
    
    float3 lightSpacePos = mul(worldPos, cascade.viewProjection[layer]);
    
    float2 shadowTexCoords;
    shadowTexCoords.x = 0.5f + (lightSpacePos.x * 0.5f);
    shadowTexCoords.y = 0.5f - (lightSpacePos.y * 0.5f);
    float pixelDepth = lightSpacePos.z;
    
    float shadow = 0;
    //shadow = ShadowMap.SampleCmpLevelZero(ShadowSampler, float3(shadowTexCoords, layer), pixelDepth);
    
    uint mapWidth, mapHeight, elem;
    ShadowMap.GetDimensions(mapWidth, mapHeight, elem);
    float2 pixelSize = float2(1.0f / mapWidth, 1.0f / mapHeight);
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            float2 pixel = float2(x, y) * pixelSize + shadowTexCoords;
            shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, float3(pixel, layer), pixelDepth);
        }
    }
    shadow /= 9;
    
    float3 viewDir = normalize(cameraPos.xyz - worldPos.xyz);
    
    float3 diffuse = max(0, dot(-light.direction.xyz, normal));
    float3 reflection = normalize(reflect(light.direction.xyz, normal));
    float3 specular = specularAlpha.xyz * pow(max(0, dot(reflection, viewDir)), specularAlpha.aaa);
    float3 lighting = light.intensity.xyz * saturate(diffuse + specular);
    
    //return float4(lighting * shadow, 1);
    return float4(lighting * shadow, 1);
}