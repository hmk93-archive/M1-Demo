#include "pch.h"
#include "DepthStencil.h"
#include "Device.h"

DepthStencil::DepthStencil(bool isStencil, UINT width, UINT height)
{
	// Texture
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = isStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		if (FAILED(Device::Get().GetDevice()->CreateTexture2D(&desc, nullptr, _dsvTexture.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = isStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		if (FAILED(Device::Get().GetDevice()->CreateDepthStencilView(_dsvTexture.Get(), &desc, _dsv.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = isStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		if (FAILED(Device::Get().GetDevice()->CreateShaderResourceView(_dsvTexture.Get(), &desc, _srv.GetAddressOf())))
		{
			__debugbreak();
		}
	}
}

DepthStencil::~DepthStencil()
{
}

void DepthStencil::Clear()
{
	Device::Get().GetDeviceContext()->ClearDepthStencilView(_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

