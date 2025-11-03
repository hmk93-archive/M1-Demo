#include "pch.h"
#include "SamplerState.h"
#include "Device.h"
#include "DepthStencilState.h"

SamplerState::SamplerState()
	: _desc{}
	, _state(nullptr)
{
	_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	_desc.MinLOD = 0;
	_desc.MaxLOD = D3D11_FLOAT32_MAX;

	Changed();
}

SamplerState::~SamplerState()
{
	_state->Release();
}

void SamplerState::SetState(UINT slot)
{
	Device::Get().GetDeviceContext()->PSSetSamplers(slot, 1, &_state);
	Device::Get().GetDeviceContext()->DSSetSamplers(slot, 1, &_state);
}

void SamplerState::Filter(D3D11_FILTER value)
{
	_desc.Filter = value;
	Changed();
}

void SamplerState::Address(D3D11_TEXTURE_ADDRESS_MODE value)
{
	_desc.AddressU = value;
	_desc.AddressV = value;
	_desc.AddressW = value;
	Changed();
}

void SamplerState::Changed()
{
	if (_state != nullptr)
		_state->Release();

	if (FAILED(Device::Get().GetDevice()->CreateSamplerState(&_desc, &_state)))
	{
		__debugbreak();
	}
}