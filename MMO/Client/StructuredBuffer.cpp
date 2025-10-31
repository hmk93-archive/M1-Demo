#include "pch.h"
#include "StructuredBuffer.h"
#include "Device.h"

StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
	: inputData(inputData), inputStride(inputStride), inputCount(inputCount),
	outputStride(outputStride), outputCount(outputCount)
{
	if (outputStride == 0 || outputCount == 0)
	{
		this->outputStride = inputStride;
		this->outputCount = inputCount;
	}

	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

StructuredBuffer::~StructuredBuffer()
{
	_input->Release();
	srv->Release();
	output->Release();
	uav->Release();
	result->Release();
}

void StructuredBuffer::Copy(void* data, UINT size)
{
	Device::Get().GetDeviceContext()->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	Device::Get().GetDeviceContext()->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	memcpy(data, subResource.pData, size);
	Device::Get().GetDeviceContext()->Unmap(result, 0);
}

void StructuredBuffer::CreateInput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = inputStride * inputCount;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = inputStride;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = inputData;

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, &initData, &buffer)))
	{
		__debugbreak();
	}

	_input = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)_input;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.NumElements = inputCount;

	if (FAILED(Device::Get().GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv)))
	{
		__debugbreak();
	}
}

void StructuredBuffer::CreateOutput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = outputStride * outputCount;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = outputStride;	

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, nullptr, &buffer)))
	{
		__debugbreak();
	}

	output = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = outputCount;

	if (FAILED(Device::Get().GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav)))
	{
		__debugbreak();
	}
}

void StructuredBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	((ID3D11Buffer*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;	
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(Device::Get().GetDevice()->CreateBuffer(&desc, nullptr, &buffer)))
	{
		__debugbreak();
	}

	result = (ID3D11Resource*)buffer;
}
