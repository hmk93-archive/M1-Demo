#include "pch.h"
#include "ConstBuffer.h"
#include "Device.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	: _data(data)
	, _dataSize(dataSize)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
	{
		__debugbreak();
	}
}

ConstBuffer::~ConstBuffer()
{
}

void ConstBuffer::Update()
{
	Device::Get().GetDeviceContext()->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &_subResource);
	memcpy(_subResource.pData, _data, _dataSize);
	Device::Get().GetDeviceContext()->Unmap(_buffer.Get(), 0);
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->VSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->PSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}

void ConstBuffer::SetCSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->CSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}

void ConstBuffer::SetHSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->HSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}

void ConstBuffer::SetDSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->DSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}

void ConstBuffer::SetGSBuffer(UINT slot)
{
	Update();
	Device::Get().GetDeviceContext()->GSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
}