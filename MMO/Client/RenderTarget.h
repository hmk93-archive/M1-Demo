#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = g_screenWidth, UINT height = g_screenHeight, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	~RenderTarget();

private:
	UINT _width = 0;
	UINT _height = 0;
};

