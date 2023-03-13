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
    float3 view : POSITION1;
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

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
    
    
    output.view = input.pos;
    output.pos = mul(float4(input.pos.xyz, 1.0f), worldViewPos);
    output.color = input.color;
    output.uv = input.uv;
    output.normal = mul(float4(input.normal, 0.0f), world);
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float3 normal = normalize(input.normal);
    //return float4(normal, 1);
    
    float3 viewDir = normalize(cameraPos.xyz - mul(float4(input.view.xyz, 1.0f), world).xyz);
    
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
    
    float3 lighting = light.intensity.xyz * saturate(diffuse + specular + ambient);
    
    color = float4(lighting, 1) * color;
    
    
 #endif   
    return color;
}