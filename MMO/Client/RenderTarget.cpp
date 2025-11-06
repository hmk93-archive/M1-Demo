#include "pch.h"
#include "RenderTarget.h"
#include "Device.h"
#include "Environment.h"
#include "DepthStencil.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
	: _width(width)
	, _height(height)
{
	// Texture
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		if (FAILED(Device::Get().GetDevice()->CreateTexture2D(&desc, nullptr, _texture.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		if (FAILED(Device::Get().GetDevice()->CreateRenderTargetView(_texture.Get(), &desc, _rtv.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		if (FAILED(Device::Get().GetDevice()->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf())))
		{
			__debugbreak();
		}
	}
}

RenderTarget::RenderTarget(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool useMSAA, UINT numQualityLevel)
	: _width(width)
	, _height(height)
{
	// Texture
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		if (useMSAA)
		{
			desc.SampleDesc.Count = 4;
			desc.SampleDesc.Quality = numQualityLevel - 1;
		}
		else
		{
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}
		if (FAILED(device->CreateTexture2D(&desc, nullptr, _texture.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = format;
		desc.ViewDimension = useMSAA ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
		if (FAILED(device->CreateRenderTargetView(_texture.Get(), &desc, _rtv.GetAddressOf())))
		{
			__debugbreak();
		}
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = format;
		desc.ViewDimension = useMSAA ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		if (FAILED(device->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf())))
		{
			__debugbreak();
		}
	}
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Set(DepthStencil* depthStencil)
{
	Device::Get().GetDeviceContext()->OMSetRenderTargets(1, _rtv.GetAddressOf(), depthStencil->GetDSV());
	Vector4 color = {};
	Device::Get().GetDeviceContext()->ClearRenderTargetView(_rtv.Get(), (float*)&color);
	depthStencil->Clear();

	Environment::Get().SetViewport(_width, _height);
}

void RenderTarget::Sets(RenderTarget** renderTargets, UINT count, DepthStencil* depthStencil)
{
	vector<ID3D11RenderTargetView*> rtvs;

	float color[4] = { 0, 0, 0, 0 };
	for (UINT i = 0; i < count; i++)
	{
		rtvs.emplace_back(renderTargets[i]->GetRTV());
		Device::Get().GetDeviceContext()->ClearRenderTargetView(renderTargets[i]->GetRTV(), color);
	}

	depthStencil->Clear();
	Device::Get().GetDeviceContext()->OMSetRenderTargets((UINT)rtvs.size(), rtvs.data(), depthStencil->GetDSV());
}
