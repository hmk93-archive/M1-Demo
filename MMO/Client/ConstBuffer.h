#pragma once
class ConstBuffer
{
public:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

	void Update();

	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);
	void SetHSBuffer(UINT slot);
	void SetDSBuffer(UINT slot);
	void SetGSBuffer(UINT slot);

private:
	ComPtr<ID3D11Buffer> _buffer;

	void* _data = nullptr;
	UINT _dataSize = 0;

	D3D11_MAPPED_SUBRESOURCE _subResource = {};
};

