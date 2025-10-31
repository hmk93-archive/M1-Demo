#include "pch.h"
#include "IndexBuffer.h"
#include "Device.h"

IndexBuffer::IndexBuffer(void* data, UINT count)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data;

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, &initData, &_buffer)))
	{
		__debugbreak();
	}
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::IASet()
{
	Device::Get().GetDeviceContext()->IASetIndexBuffer(_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
