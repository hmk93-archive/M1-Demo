#include "Common.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    output.alpha = input.alpha;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = 0;
    if (hasDiffuseMap)
        albedo = diffuseMap.Sample(linearWrapSS, input.uv);
    
    return albedo;
}