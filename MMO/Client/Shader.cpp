#include "pch.h"
#include "Shader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "DomainShader.h"
#include "HullShader.h"
#include "GeometryShader.h"
#include "Utility.h"
#include "Texture.h"
using namespace Utility;

unordered_map<wstring, Shader*> Shader::s_shaderCache;

VertexShader* Shader::AddVS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (VertexShader*)s_shaderCache[key];

    s_shaderCache[key] = new VertexShader(file, entry);

    return (VertexShader*)s_shaderCache[key];
}

PixelShader* Shader::AddPS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (PixelShader*)s_shaderCache[key];

    s_shaderCache[key] = new PixelShader(file, entry);

    return (PixelShader*)s_shaderCache[key];
}

ComputeShader* Shader::AddCS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (ComputeShader*)s_shaderCache[key];

    s_shaderCache[key] = new ComputeShader(file, entry);

    return (ComputeShader*)s_shaderCache[key];
}

HullShader* Shader::AddHS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (HullShader*)s_shaderCache[key];

    s_shaderCache[key] = new HullShader(file, entry);

    return (HullShader*)s_shaderCache[key];
}

DomainShader* Shader::AddDS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (DomainShader*)s_shaderCache[key];

    s_shaderCache[key] = new DomainShader(file, entry);

    return (DomainShader*)s_shaderCache[key];
}

GeometryShader* Shader::AddGS(wstring file, string entry)
{
    wstring key = file + ToWString(entry);

    if (s_shaderCache.count(key) > 0)
        return (GeometryShader*)s_shaderCache[key];

    s_shaderCache[key] = new GeometryShader(file, entry);

    return (GeometryShader*)s_shaderCache[key];
}

void Shader::Delete()
{
    for (auto shader : s_shaderCache)
        delete shader.second;
}
