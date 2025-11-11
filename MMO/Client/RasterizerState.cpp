#include "pch.h"
#include "RasterizerState.h"
#include "Device.h"

RasterizerState::RasterizerState()
{
	_desc.CullMode = D3D11_CULL_BACK;
	_desc.FillMode = D3D11_FILL_SOLID;

	Changed();
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::SetState()
{
	Device::Get().GetDeviceContext()->RSSetState(_state.Get());
}

void RasterizerState::FillMode(D3D11_FILL_MODE mode)
{
	_desc.FillMode = mode;
	Changed();
}

void RasterizerState::CullMode(D3D11_CULL_MODE mode)
{
	_desc.CullMode = mode;
	Changed();
}

void RasterizerState::Changed()
{
	if (_state)
		_state->Release();

	if (FAILED(Device::Get().GetDevice()->CreateRasterizerState(&_desc, _state.GetAddressOf())))
	{
		__debugbreak();
	}
}

void RasterizerState::FrontCounterClockwise(bool isClock)
{
	_desc.FrontCounterClockwise = isClock;

	Changed();
}

