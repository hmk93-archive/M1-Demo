#pragma once

class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void IASet();

private:
	ComPtr<ID3D11Buffer> _buffer;
};

