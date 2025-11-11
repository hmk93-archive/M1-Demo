#include "Common.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = albedoMap.Sample(linearWrapSS, input.uv);
    return color;
}