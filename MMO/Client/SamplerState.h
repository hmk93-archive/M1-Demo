#pragma once

class SamplerState
{
public:
	SamplerState();
	~SamplerState();

	void SetState(UINT slot = 0);

	void Filter(D3D11_FILTER value);
	void Address(D3D11_TEXTURE_ADDRESS_MODE value);

private:
	void Changed();

private:
	D3D11_SAMPLER_DESC _desc;
	ID3D11SamplerState* _state;
};
