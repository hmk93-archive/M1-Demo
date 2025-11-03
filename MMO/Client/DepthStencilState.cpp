#include "pch.h"
#include "DepthStencilState.h"
#include "Device.h"

DepthStencilState::DepthStencilState()
{
	_desc.DepthEnable = true;
	_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	_desc.DepthFunc = D3D11_COMPARISON_LESS;

	_desc.StencilEnable = true;
	_desc.StencilReadMask = 0xff;
	_desc.StencilWriteMask = 0xff;

	_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Changed();
}

DepthStencilState::~DepthStencilState()
{
}

void DepthStencilState::SetState(UINT stencilRef)
{
	Device::Get().GetDeviceContext()->OMSetDepthStencilState(_state.Get(), stencilRef);
}

void DepthStencilState::DepthEnable(bool enable)
{
	_desc.DepthEnable = enable;
	Changed();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK mask)
{
	_desc.DepthWriteMask = mask;
	Changed();
}

void DepthStencilState::DepthFunc(D3D11_COMPARISON_FUNC func)
{
	_desc.DepthFunc = func;
	Changed();
}

void DepthStencilState::Changed()
{
	if (_state != nullptr)
		_state->Release();

	if (FAILED(Device::Get().GetDevice()->CreateDepthStencilState(&_desc, _state.GetAddressOf())))
	{
		__debugbreak();
	}
}
