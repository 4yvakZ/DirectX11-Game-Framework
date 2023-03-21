#pragma pack_matrix( row_major )

Texture2D DiffuseMap : register(t0);
Texture2DArray ShadowMap : register(t1);

SamplerState Sampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

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
    float3 viewPos : POSITION1;
    float3 worldPos : POSITION2;
};

cbuffer OBJECT_CONST_BUF : register(b0)
{
    Matrix worldViewPos;
    Matrix world;
    float4 cameraPos;
}

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

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
    
    
    output.viewPos = input.pos;
    output.pos = mul(float4(input.pos.xyz, 1.0f), worldViewPos);
    output.color = input.color;
    output.uv = input.uv;
    output.normal = mul(float4(input.normal, 0.0f), world);
    
    output.worldPos = mul(float4(input.pos.xyz, 1.0f), world).xyz;
    
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    int layer = getCascadeLayer(abs(input.viewPos.z));
    //layer = 1;
    
    float3 lightSpacePos = mul(float4(input.worldPos, 1.0f), cascade.viewProjection[layer]);
    
    float2 shadowTexCoords;
    shadowTexCoords.x = 0.5f + (lightSpacePos.x * 0.5f);
    shadowTexCoords.y = 0.5f - (lightSpacePos.y * 0.5f);
    float pixelDepth = lightSpacePos.z;
    
    float shadow = 0;
    shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, float3(shadowTexCoords, layer), pixelDepth);
    //return shadow;
    
    /*uint width, height, elem;
    ShadowMap.GetDimensions(width, height, elem);
    float2 pixelSize = float2(1.0f / width, 1.0f / height);
    for (int x = -2; x < 3; x++)
    {
        for (int y = -2; y < 3; y++)
        {
            float2 pixel = float2(x, y) * pixelSize + shadowTexCoords;
            shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, float3(pixel, 0), pixelDepth);
        }
    }
    shadow /= 25;*/
    
    //return shadow;
    
    float3 normal = normalize(input.normal);
    //return float4(normal, 1);
    
    float3 viewDir = normalize(cameraPos.xyz - mul(float4(input.viewPos.xyz, 1.0f), world).xyz);
    
    float4 color;   

#if PLAIN
    color = input.color;
#else
    color = DiffuseMap.Sample(Sampler, input.uv.xy);
       
    
    float3 diffuse = material.diffuse.xyz * max(0, dot(-light.direction.xyz, normal));
    //diffuse = float3(0, 0, 0);
    
    float3 ambient = material.ambient.xyz ;
    //ambient = float3(0, 0, 0);
    
    float3 reflection = normalize(reflect(light.direction.xyz, normal));
    float3 specular = material.specularAlpha.xyz * pow(max(0, dot(reflection, viewDir)), material.specularAlpha.aaa);
    //specular = float3(0, 0, 0);
    
    float3 lighting = light.intensity.xyz * saturate(shadow * (diffuse + specular) + ambient);
    
    color = float4(lighting, 1) * color;
    
    
 #endif   
    return color;
}