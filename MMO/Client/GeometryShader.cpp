#include "pch.h"
#include "GeometryShader.h"
#include "Device.h"

GeometryShader::GeometryShader(wstring file, string entry)
{
	wstring path = L"../Shaders/" + file + L".hlsl";

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry.c_str(),
		"gs_5_0",
		flags,
		0,
		&_blob, nullptr)))
	{
		__debugbreak();
	}

	if (FAILED(Device::Get().GetDevice()->CreateGeometryShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _shader.GetAddressOf())))
	{
		__debugbreak();
	}

	_blob->Release();
	_blob = nullptr;
}

GeometryShader::~GeometryShader()
{
	_shader->Release();
}

void GeometryShader::Set()
{
	Device::Get().GetDeviceContext()->GSSetShader(_shader.Get(), nullptr, 0);
}
