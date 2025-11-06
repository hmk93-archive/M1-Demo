#pragma once

class DepthStencil
{
public:
	DepthStencil(bool isStencil = false, UINT width = g_screenWidth, UINT height = g_screenHeight);
	~DepthStencil();

	void Clear();

	ID3D11DepthStencilView* GetDSV() { return _dsv.Get(); }
	ID3D11ShaderResourceView* GetSRV() { return _srv.Get(); }

private:
	ComPtr<ID3D11Texture2D> _dsvTexture;
	ComPtr<ID3D11DepthStencilView> _dsv;
	ComPtr<ID3D11ShaderResourceView> _srv;
};