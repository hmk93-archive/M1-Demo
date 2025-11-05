#pragma once

class BlendState
{
public:
	BlendState();
	~BlendState();

	void SetState();

	void Alpha(bool value);
	void AlphaToCoverage(bool value);
	void Additive();

	void Changed();

private:
	ComPtr<ID3D11BlendState> state;
	D3D11_BLEND_DESC desc = {};
};