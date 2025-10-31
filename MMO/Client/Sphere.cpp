#include "pch.h"
#include "Sphere.h"
#include "Material.h"
#include "Mesh.h"
#include "Device.h"

Sphere::Sphere(wstring file, float radius, UINT sliceCount, UINT stackCount)
	: _radius(radius)
	, _sliceCount(sliceCount)
	, _stackCount(stackCount)
{
	_material = new Material(file);
	CreateMesh();
}

Sphere::~Sphere()
{
	delete _mesh;
	delete _material;
}

void Sphere::Update()
{

}

void Sphere::Render()
{
	_mesh->IASet();
	_material->Set();
	SetWorldBuffer();

	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void Sphere::PostRender()
{

}

void Sphere::CreateMesh()
{
	float phiStep = XM_PI / _stackCount;
	float thetaStep = XM_2PI / _sliceCount;

	for (UINT i = 0; i <= _stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= _sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexUVNormalTangent vertex;

			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.position = Vector3(vertex.normal) * _radius;

			vertex.uv.x = (float)j / _sliceCount;
			vertex.uv.y = (float)i / _stackCount;

			_vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < _stackCount; i++)
	{
		for (UINT j = 0; j < _sliceCount; j++)
		{
			_indices.emplace_back((_sliceCount + 1) * i + j);
			_indices.emplace_back((_sliceCount + 1) * i + j + 1);
			_indices.emplace_back((_sliceCount + 1) * (i + 1) + j);

			_indices.emplace_back((_sliceCount + 1) * (i + 1) + j);
			_indices.emplace_back((_sliceCount + 1) * i + j + 1);
			_indices.emplace_back((_sliceCount + 1) * (i + 1) + j + 1);
		}
	}

	CreateTangent();

	_mesh = new Mesh(_vertices.data(), sizeof(VertexUVNormalTangent), _vertices.size(), _indices.data(), _indices.size());
}

void Sphere::CreateTangent()
{
	for (UINT i = 0; i < _indices.size() / 3; i++)
	{
		UINT index0 = _indices[i * 3 + 0];
		UINT index1 = _indices[i * 3 + 1];
		UINT index2 = _indices[i * 3 + 2];

		VertexUVNormalTangent vertex0 = _vertices[index0];
		VertexUVNormalTangent vertex1 = _vertices[index1];
		VertexUVNormalTangent vertex2 = _vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Vector2 uv0 = vertex0.uv;
		Vector2 uv1 = vertex1.uv;
		Vector2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		Vector3 tangent = (v1 * e0 - v0 * e1);

		_vertices[index0].tangent = tangent + _vertices[index0].tangent;
		_vertices[index1].tangent = tangent + _vertices[index1].tangent;
		_vertices[index2].tangent = tangent + _vertices[index2].tangent;
	}

	for (VertexUVNormalTangent& vertex : _vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * n.Dot(t));
		temp.Normalize();

		vertex.tangent = temp;
	}
}