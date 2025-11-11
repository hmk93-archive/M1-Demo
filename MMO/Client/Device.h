#pragma once

class RenderTarget;

class Device
{
	DECLARE_SINGLETON(Device);
public:
	void SetRenderTarget();
	void Clear(Vector4 color = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	void Present();

	ID3D11Device* GetDevice() { return _device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() { return _swapChain.Get(); }
	
	ID3D11RenderTargetView* GetBackRT() { return _backRTV.Get(); }
	RenderTarget* GetFloatRT() { return _floatRTV; }
	RenderTarget* GetResolvedRT() { return _resolvedRTV; }

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
	
	ComPtr<ID3D11RenderTargetView> _backRTV;
	ComPtr<ID3D11DepthStencilView> _backDSV;

	bool _useMSAA = false;
	UINT _numQualityLevel = 0;

	RenderTarget* _floatRTV = nullptr;
	RenderTarget* _resolvedRTV = nullptr;
};

