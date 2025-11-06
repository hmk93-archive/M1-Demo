#include "Common.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 alpha : ALPHA;
    float3 eyePos : EYEPOS;
    float3 posWorld : POSITION;
};

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    
    output.eyePos = invView._41_42_43;
    output.posWorld = output.pos.xyz;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
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
    
    Material material;
    material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    material.diffuseColor = albedo;
    material.eyePos = input.eyePos;
    material.emissive = mEmissive;
    material.shininess = shininess;
    material.specularIntensity = SpecularMapping(input.uv);
    material.posWorld = input.posWorld;
    
    float4 ambient = CalcAmbient(material);
    float4 result = 0;
    for (uint i = 0; i < lightCount; i++)
    {
        if (!lights[i].active)
            continue;
        [flatten]
        if (lights[i].type == 0)
            result += CalcDirectional(material, lights[i]);

    }
    
    float4 emissive = CalcEmissive(material);
    
    float4 second = secondMap.Sample(linearWrapSS, input.uv);
    float4 third = thirdMap.Sample(linearWrapSS, input.uv);
    
    result = lerp(result, second, input.alpha.r);
    result = lerp(result, third, input.alpha.g);
    
    return result;
}