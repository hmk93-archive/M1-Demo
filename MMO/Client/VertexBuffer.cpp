#include "pch.h"
#include "VertexBuffer.h"
#include "Device.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count, bool isCPUWrite)
	: _stride(stride)
{
	D3D11_BUFFER_DESC desc = {};
	if (isCPUWrite)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
	}

	desc.ByteWidth = stride * count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data;

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, &initData, _buffer.GetAddressOf())))
	{
		__debugbreak();
	}
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::IASet(UINT slot)
{
	Device::Get().GetDeviceContext()->IASetVertexBuffers(slot, 1, _buffer.GetAddressOf(), &_stride, &_offset);
}

void VertexBuffer::Update(void* data, UINT count)
{
	Device::Get().GetDeviceContext()->UpdateSubresource(_buffer.Get(), 0, nullptr, data, _stride, count);
}

void VertexBuffer::Map(void* data, UINT start, UINT size, UINT offset)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	subResource.pData = data;

	Device::Get().GetDeviceContext()->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);
	memcpy((BYTE*)subResource.pData + offset, (BYTE*)data + start, size);
	Device::Get().GetDeviceContext()->Unmap(_buffer.Get(), 0);
}
