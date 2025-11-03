#pragma once

class DepthStencilState
{
public:
	DepthStencilState();
	~DepthStencilState();

	void SetState(UINT stencilRef = 1);

	void DepthEnable(bool enable);
	void DepthWriteMask(D3D11_DEPTH_WRITE_MASK mask);
	void DepthFunc(D3D11_COMPARISON_FUNC func);

private:
	void Changed();

private:
	D3D11_DEPTH_STENCIL_DESC _desc = {};
	ComPtr<ID3D11DepthStencilState> _state;
};

