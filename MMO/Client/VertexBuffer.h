#pragma once

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT stride, UINT count, bool isCPUWrite = false);
	~VertexBuffer();

	void IASet(UINT slot = 0);

	void Update(void* data, UINT count);
	void Map(void* data, UINT start, UINT size, UINT offset);

	ID3D11Buffer* GetBuffer() { return _buffer.Get(); }

private:
	ComPtr<ID3D11Buffer> _buffer;
	UINT _stride;
	UINT _offset = 0;
};

