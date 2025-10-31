#include "pch.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

Mesh::Mesh(void* vertexData, UINT stride, UINT vertexCount, void* indexData, UINT indexCount, bool isCPUWrite)
{
	_vertexBuffer = new VertexBuffer(vertexData, stride, vertexCount, isCPUWrite);
	_indexBuffer = new IndexBuffer(indexData, indexCount);
}

Mesh::~Mesh()
{
	delete _vertexBuffer;
	delete _indexBuffer;
}

void Mesh::IASet(D3D11_PRIMITIVE_TOPOLOGY primitiveType)
{
	_vertexBuffer->IASet();
	_indexBuffer->IASet();
	Device::Get().GetDeviceContext()->IASetPrimitiveTopology(primitiveType);
}

void Mesh::UpdateVertex(void* data, UINT count)
{
	_vertexBuffer->Update(data, count);
}

void Mesh::MapVertex(void* data, UINT start, UINT size, UINT offset)
{
	_vertexBuffer->Map(data, start, size, offset);
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
    return _vertexBuffer->GetBuffer();
}
