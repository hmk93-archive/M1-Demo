#include "Common.hlsli"

cbuffer Brush : register(b10)
{
    int type;
    float3 location;
    float range;
    float3 color;
}

struct PixelInput
{
    float4 posProj : SV_POSITION;
    float4 posWorld : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

float3 BrushColor(float3 pos)
{
    if(type == 0)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        float distance = sqrt(x * x + z * z);
        if(distance <= range)
            return color;
    }
    return 0;
}

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    
    output.posWorld = mul(input.pos, world);
    output.posProj = mul(output.posWorld, view);
    output.posProj = mul(output.posProj, projection);
    
    output.uv = input.uv;
    output.alpha = input.alpha;
    
    return output;
}

Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = 1;
    if (hasAlbedoMap)
        albedo = albedoMap.Sample(linearWrapSS, input.uv);
    
    float4 second = secondMap.Sample(linearWrapSS, input.uv);
    float4 third = thirdMap.Sample(linearWrapSS, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    
    float4 brushColor = float4(BrushColor(input.posWorld.xyz), 1.0);
    
    return albedo + brushColor;
}