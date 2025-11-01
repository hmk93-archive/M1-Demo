#define MAX_BONE 256
#define MAX_LIGHT 10
#define MAX_INSTANCE 400

// Vertex shader buffer
cbuffer World : register(b0)
{
    matrix world;
}

cbuffer View : register(b1)
{
    matrix view;
    matrix invView;
}

cbuffer Projection : register(b2)
{
    matrix projection;
}

cbuffer Bone : register(b3)
{
    matrix bones[MAX_BONE];
}

struct KeyFrame
{
    int clip;
    uint curFrame;
    uint nextFrame;
    float time;
    
    float runninTime;
    float speed;
    float2 padding;
};

struct TweenFrame
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    KeyFrame cur;
    KeyFrame next;
};

cbuffer Frame : register(b4)
{
    TweenFrame tweenFrame[MAX_INSTANCE];
}

cbuffer ModelType : register(b5)
{
    int modelType;
}

Texture2DArray transformMap : register(t0);

// Pixel Shader
cbuffer MaterialInfo : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
    
    float shininess;
    
    int hasDiffuseMap;
    int hasSpecularMap;
    int hasNormalMap;
}

SamplerState linearWrapSS : register(s0);
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

// Vertex layouts
struct Vertex
{
    float4 pos : POSITION;
};

struct VertexSize
{
    float3 pos : POSITION;
    float2 size : SIZE;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentAlpha
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHT;
};

struct VertexInstancing
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHT;
    
    matrix transform : INSTANCE_TRANSFORM;
    uint index : INSTANCE_INDEX;
    uint instanceID : SV_INSTANCEID;
};

matrix BoneWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    
    transform += mul(weights.x, bones[(uint)indices.x]);
    transform += mul(weights.y, bones[(uint)indices.y]);
    transform += mul(weights.z, bones[(uint)indices.z]);
    transform += mul(weights.w, bones[(uint)indices.w]);
    
    return transform;
}

matrix SkinWorld(float4 indices, float4 weights)
{
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;
    matrix cur = 0, next = 0;
    matrix curAnim = 0, nextAnim = 0;
    
    int clip[2];
    int curFrame[2];
    int nextFrame[2];
    float time[2];
    
    clip[0] = tweenFrame[0].cur.clip;
    curFrame[0] = tweenFrame[0].cur.curFrame;
    nextFrame[0] = tweenFrame[0].cur.nextFrame;
    time[0] = tweenFrame[0].cur.time;
    
    clip[1] = tweenFrame[0].next.clip;
    curFrame[1] = tweenFrame[0].next.curFrame;
    nextFrame[1] = tweenFrame[0].next.nextFrame;
    time[1] = tweenFrame[0].next.time;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clip[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clip[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clip[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clip[0], 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, time[0]);
        
        [flatten]
        if (clip[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clip[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clip[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clip[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clip[1], 0));
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, tweenFrame[0].tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }

    return transform;
}
