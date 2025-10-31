#include "pch.h"
#include "ComputeShader.h"
#include "Device.h"

ComputeShader::ComputeShader(wstring file, string entry)
{
	wstring path = L"../Shaders/" + file + L".hlsl";

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry.c_str(),
		"cs_5_0",
		flags,
		0,
		&_blob, nullptr)))
	{
		__debugbreak();
	}

	if (FAILED(Device::Get().GetDevice()->CreateComputeShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _shader.GetAddressOf())))
	{
		__debugbreak();
	}

	_blob->Release();
	_blob = nullptr;
}

ComputeShader::~ComputeShader()
{
	_shader->Release();
}

void ComputeShader::Set()
{
	Device::Get().GetDeviceContext()->CSSetShader(_shader.Get(), nullptr, 0);
}
