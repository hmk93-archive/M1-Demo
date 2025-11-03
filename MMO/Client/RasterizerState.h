#pragma once

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	void SetState();
	void FillMode(D3D11_FILL_MODE mode);
	void FrontCounterClockwise(bool isClock);
	void Changed();
private:
	D3D11_RASTERIZER_DESC _desc = {};
	ComPtr<ID3D11RasterizerState> _state = {};
};

