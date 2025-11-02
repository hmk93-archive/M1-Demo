#include "Common.hlsli"

struct PixelInput
{
    float4 posProj : SV_POSITION;
    float4 posWorld : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

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
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(linearWrapSS, input.uv);
    
    float4 second = secondMap.Sample(linearWrapSS, input.uv);
    float4 third = thirdMap.Sample(linearWrapSS, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    
    return albedo;
}