#include "Common.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 eyePos : EYEPOS;
    float3 posWorld : POSITION0;
    float4 posClip : POSITION1;
};

PixelInput VS(VertexInstancing input)
{
    PixelInput output;
    
    matrix transform = 0;
    
    [flatten]
    if (modelType == 2)
        transform = mul(SkinWorld(input.index, input.indices, input.weights), input.transform);
    else if (modelType == 1)
        transform = mul(BoneWorld(input.indices, input.weights), input.transform);
    else
        transform = input.transform;
    
    output.pos = mul(input.pos, transform);
    
    output.eyePos = invView._41_42_43;
    output.posWorld = output.pos.xyz;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.posClip = mul(input.pos, transform);
    output.posClip = mul(output.posClip, lightView);
    output.posClip = mul(output.posClip, lightProjection);
    
    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

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
    
    float4 color = result + ambient + emissive;
    
    float currentDepth = input.posClip.z / input.posClip.w;
    float2 uv = input.posClip.xy / input.posClip.w;
    uv.y = -uv.y;
    uv = uv * 0.5 + 0.5;
    
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
        return color;
    
    if (currentDepth < 0.0 || currentDepth > 1.0)
        return color;
    
    float shadowDepth = depthMap.Sample(linearWrapSS, uv).r;
    float factor = 0.0;
    
    [flatten]
    if (quality == 0)
    {
        if (currentDepth > shadowDepth + 0.0001f)
            factor = 0.5;
    }
    
    factor = saturate(factor);
    if (factor < 1.0)
        factor = 1.0 - factor;
    
    return color * factor;
}