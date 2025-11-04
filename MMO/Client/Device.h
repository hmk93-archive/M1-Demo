#pragma once

class Device
{
	DECLARE_SINGLETON(Device);
public:
	void SetRenderTarget();
	void Clear(Vector4 color = Vector4(0.0f, 0.125f, 0.8f, 1.0f));
	void Present();

	ID3D11Device* GetDevice() { return _device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() { return _swapChain.Get(); }

	void UpdateWindowSize(UINT width, UINT height);

private:
	void CreateDeviceAndSwapchain();
	void CreateRenderTarget();
	void CreateDepthStencil();

	void Debug();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
};

