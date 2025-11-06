#pragma once

class DepthStencil;

class RenderTarget
{
public:
	RenderTarget(UINT width = g_screenWidth, UINT height = g_screenHeight, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	RenderTarget(ID3D11Device* device, UINT width = g_screenWidth, UINT height = g_screenHeight, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT,
		bool useMSAA = false, UINT numQualityLevel = 0);
	~RenderTarget();

	void Set(DepthStencil* depthStencil);
	
	static void Sets(RenderTarget** renderTargets, UINT count, DepthStencil* depthStencil);

	ID3D11ShaderResourceView* GetSRV() { return _srv.Get(); }
	ID3D11RenderTargetView* GetRTV() { return _rtv.Get(); }

	ID3D11Texture2D* GetResource() { return _texture.Get(); }

private:
	UINT _width = 0;
	UINT _height = 0;

	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _srv;
	ComPtr<ID3D11RenderTargetView> _rtv;
};

