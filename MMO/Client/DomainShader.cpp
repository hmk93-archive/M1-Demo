#include "pch.h"
#include "DomainShader.h"
#include "Device.h"

DomainShader::DomainShader(wstring file, string entry)
{
	wstring path = L"../Shaders/" + file + L".hlsl";

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry.c_str(),
		"ds_5_0",
		flags,
		0,
		&_blob, nullptr)))
	{
		__debugbreak();
	}

	if (FAILED(Device::Get().GetDevice()->CreateDomainShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _shader.GetAddressOf())))
	{
		__debugbreak();
	}

	_blob->Release();
	_blob = nullptr;
}

DomainShader::~DomainShader()
{
	_shader->Release();
}

void DomainShader::Set()
{
	Device::Get().GetDeviceContext()->DSSetShader(_shader.Get(), nullptr, 0);
}
