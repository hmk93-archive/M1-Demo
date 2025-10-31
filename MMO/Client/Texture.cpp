#include "pch.h"
#include "Texture.h"
#include "Utility.h"
#include "Device.h"
using namespace Utility;

map<wstring, Texture*> Texture::s_textureCache;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
	: _srv(srv)
	, _image(move(image))
{
}

Texture::~Texture()
{
}

Texture* Texture::Add(wstring file)
{
    if (s_textureCache.count(file) > 0)
        return s_textureCache[file];

    ScratchImage image;

    wstring extension = GetExtension(file);

    if (extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    ComPtr<ID3D11ShaderResourceView> srv;

    if (FAILED(CreateShaderResourceView(Device::Get().GetDevice(), image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv)))
    {
        __debugbreak();
    }

    s_textureCache[file] = new Texture(srv.Get(), image);

    return s_textureCache[file];
}

Texture* Texture::Load(wstring file)
{
    ScratchImage image;

    wstring extension = GetExtension(file);

    if (extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    ComPtr<ID3D11ShaderResourceView> srv;

    if (FAILED(CreateShaderResourceView(Device::Get().GetDevice(), image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv)))
    {
        __debugbreak();
    }

    if (s_textureCache[file])
        delete s_textureCache[file];

    s_textureCache[file] = new Texture(srv.Get(), image);

    return s_textureCache[file];
}

void Texture::Delete()
{
    for (auto texture : s_textureCache)
        delete texture.second;
}

void Texture::PSSet(UINT slot)
{
    Device::Get().GetDeviceContext()->PSSetShaderResources(slot, 1, _srv.GetAddressOf());
}

void Texture::DSSet(UINT slot)
{
    Device::Get().GetDeviceContext()->DSSetShaderResources(slot, 1, _srv.GetAddressOf());
}

vector<Vector4> Texture::ReadPixels()
{
    BYTE* colors = (BYTE*)_image.GetPixels();
    UINT size = (UINT)_image.GetPixelsSize();

    float scale = 1.0f / 255.0f;
    vector<Vector4> result(size / 4);
    for (UINT i = 0; i < result.size(); i++)
    {
        result[i].x = colors[i * 4 + 0] * scale;
        result[i].y = colors[i * 4 + 1] * scale;
        result[i].z = colors[i * 4 + 2] * scale;
        result[i].w = colors[i * 4 + 3] * scale;
    }
    return result;
}

