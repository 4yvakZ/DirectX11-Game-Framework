#pragma pack_matrix( row_major )

#define THREAD_GROUP_X 32
#define THREAD_GROUP_Y 32
#define THREAD_IN_GROUP_TOTAL 1024
#define GROUP_COUNT_Y 32

cbuffer OBJECT_CONST_BUF : register(b0)
{
    Matrix view;
    Matrix projection;
    Matrix world;
    float4 cameraPos;
}

struct Emitter
{
    float minLifetima;
    float maxLifetime;
    float minSpawnVelocity;
    float maxSpawnVelocity;

    float minSize;
    float maxSize;
    float deltaTime;
    float blank0;

    uint emitCount;
    uint maxNumberOfParticles;
    uint numGroups;
    uint blank2;

    float4 force;
    float4 minSpawnPos;
    float4 maxSpawnPos;
};

cbuffer EMITTER_CONST_BUF : register(b4)
{
    Emitter emitter;
}

struct HeightMap
{
    Matrix viewProjection;
};

cbuffer HEIGHT_MAP_CB : register(b5)
{
    HeightMap heightMap;
}

struct Particle
{
    float4 position;
    float4 velocity;
    float timeUntilDeath;
    float size;
};

struct SortingData
{
    uint index;
    float depth;
};

RWStructuredBuffer<SortingData> SortedList : register(u0);
RWStructuredBuffer<Particle> Particles : register(u1);
ConsumeStructuredBuffer<uint> DeadListConsume : register(u2);
AppendStructuredBuffer<uint> DeadListAppend : register(u3);

Texture2D Texture : register(t0);
StructuredBuffer<Particle> ParticlesResoures : register(t1);
StructuredBuffer<SortingData> SortedListResoures : register(t2);
Texture2D HeightMapTex : register(t3);

SamplerState Sampler : register(s0);
SamplerComparisonState CmpSampler : register(s1);

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void EmitCS(uint3 groupID : SV_GroupID,
    uint groupIndex: SV_GroupIndex)
{
    uint index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * emitter.numGroups * THREAD_IN_GROUP_TOTAL + groupIndex;
    
    if (index >= emitter.emitCount)
    {
        return;
    }
    index = DeadListConsume.Consume();
    
    float3 rand3 = float3(frac(sin(dot(float2(index, emitter.deltaTime), float2(12.9898, 78.233))) * 43758.5453),
    frac(sin(dot(float2(index * index, index * emitter.deltaTime), float2(12.9898, 78.233)) * 2.0) * 43758.5453),
    frac(sin(dot(float2(index, index * emitter.deltaTime), float2(12.9898, 78.233)) * 2.0) * 43758.5453));
    float rand1 = frac(rand3.x + rand3.y + rand3.z);
    
    Particles[index].position = float4(((emitter.maxSpawnPos - emitter.minSpawnPos).xyz * rand3), 0.0f) + emitter.minSpawnPos;
    Particles[index].size = (emitter.maxSize - emitter.minSize) * rand1 + emitter.minSize;
    Particles[index].timeUntilDeath = (emitter.maxLifetime - emitter.minLifetima) * rand1 + emitter.minLifetima;
    Particles[index].velocity = (emitter.maxSpawnVelocity - emitter.minSpawnVelocity) * rand1 + emitter.minSpawnVelocity;
}

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void SimulateCS(uint3 groupID : SV_GroupID,
    uint groupIndex : SV_GroupIndex)
{
    uint index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * emitter.numGroups * THREAD_IN_GROUP_TOTAL + groupIndex;
   
    if (index >= emitter.maxNumberOfParticles)
    {
        return;
    }
    
    SortedList[index].index = index;
    SortedList[index].depth = 0.0f;
    
    if (Particles[index].timeUntilDeath <= 0.0f)
    {
        return;
    }
    
    Particles[index].timeUntilDeath -= emitter.deltaTime;
    
    if (Particles[index].timeUntilDeath <= 0.0f)
    {
        //Particles[index].size = 1;
        DeadListAppend.Append(index);
        return;
    }
    

    
    float3 heightSpacePos = mul(mul(Particles[index].position, world),heightMap.viewProjection).xyz;
    float2 heightTexCoords;
    heightTexCoords.x = 0.5f + (heightSpacePos.x * 0.5f);
    heightTexCoords.y = 0.5f - (heightSpacePos.y * 0.5f);
    float pixelDepth = heightSpacePos.z;
    uint width, height;
    HeightMapTex.GetDimensions(width, height);
    
    if (!HeightMapTex.SampleCmpLevelZero(CmpSampler, heightTexCoords, pixelDepth).x)
    {
        float3 rand3 = float3(frac(sin(dot(float2(index, emitter.deltaTime), float2(12.9898, 78.233))) * 43758.5453),
        frac(sin(dot(float2(index * index, index * emitter.deltaTime), float2(12.9898, 78.233)) * 2.0) * 43758.5453),
        frac(sin(dot(float2(index, index * emitter.deltaTime), float2(12.9898, 78.233)) * 2.0) * 43758.5453));
        Particles[index].velocity.y *= -0.5;
        Particles[index].velocity.x *= rand3.x;
        Particles[index].velocity.z *= rand3.z;
        Particles[index].position += Particles[index].velocity * emitter.deltaTime;
        SortedList.IncrementCounter();
        return;
    }
    
    Particles[index].velocity += emitter.deltaTime * emitter.force;
    Particles[index].position += Particles[index].velocity * emitter.deltaTime;
    
    
    SortedList[index].depth = mul(mul(Particles[index].position, world), view).z;
    
    SortedList.IncrementCounter();
}

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float life : LIFE;
};

struct GS_IN
{
    float4 viewPos : POSITION0;
    float size : SIZE;
    float life : LIFE;
};


GS_IN VSMain( uint id: SV_VertexID )
{
    GS_IN output = (GS_IN) 0;
    
    output.viewPos = mul(mul(ParticlesResoures[SortedListResoures[id].index].position, world), view);
    output.size = ParticlesResoures[SortedListResoures[id].index].size;
    output.life = ParticlesResoures[SortedListResoures[id].index].timeUntilDeath;
	return output;
}

PS_IN _offsetNprojected(PS_IN data, float2 offset, float2 uv)
{
    data.pos.xy += offset;
    data.pos = mul(data.pos, projection);
    data.tex = uv;

    return data;
}

[maxvertexcount(4)]
void GSMain(point GS_IN input[1], inout TriangleStream<PS_IN> stream)
{
    PS_IN output = (PS_IN) 0;
    output.pos = input[0].viewPos;
    output.life = input[0].life;
    
    //const float size = 0.1f; // размер конченого квадрата 
    const float size = input[0].size;
	// описание квадрата
    stream.Append(_offsetNprojected(output, float2(-1, -1) * size, float2(0, 1)));
    stream.Append(_offsetNprojected(output, float2(-1, 1) * size, float2(0, 0)));
    stream.Append(_offsetNprojected(output, float2(1, -1) * size, float2(1, 1)));
    stream.Append(_offsetNprojected(output, float2(1, 1) * size, float2(1, 0)));
    
    stream.RestartStrip();
}

float4 PSMain( PS_IN input ) : SV_Target0
{
    if (input.life <= 0)
    {
        //return float4(1, 0, 0, 1);
    }
    float4 color = Texture.Sample(Sampler, input.tex);
    clip(color.a - 0.001f);
    
    return color;
}

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};

VS_OUT HMVSMain(VS_IN input)
{  
    VS_OUT output = (VS_OUT) 0;
    output.pos = mul(mul(float4(input.pos.xyz, 1.0f), world), heightMap.viewProjection);
    
    return output;
}