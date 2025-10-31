#include "pch.h"
#include "Device.h"

Device::Device()
{
	CreateDeviceAndSwapchain();
	CreateBackBuffer();
}

Device::~Device()
{

}

void Device::CreateDeviceAndSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 2;
	desc.BufferDesc.Width = g_screenWidth;
	desc.BufferDesc.Height = g_screenHeight;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = g_hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&_swapChain,
		&_device,
		nullptr,
		&_deviceContext
	)))
	{
		__debugbreak();
	}
}

void Device::CreateBackBuffer()
{
	ID3D11Texture2D* backBuffer;

	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		__debugbreak();
	}
	if (FAILED(_device->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView)))
	{
		__debugbreak();
	}
	backBuffer->Release();

	ID3D11Texture2D* depthBuffer;

	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = g_screenWidth;
		desc.Height = g_screenHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		if (FAILED(_device->CreateTexture2D(&desc, nullptr, &depthBuffer)))
		{
			__debugbreak();
		}
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (FAILED(_device->CreateDepthStencilView(depthBuffer, &desc, &_depthStencilView)))
		{
			__debugbreak();
		}
		depthBuffer->Release();
	}
}

void Device::SetRenderTarget()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
}

void Device::Clear(Vector4 color)
{
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), (float*)&color);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
	_swapChain->Present(0, 0);
}
