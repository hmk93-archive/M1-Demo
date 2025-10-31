#include "pch.h"
#include "Cube.h"
#include "Mesh.h"
#include "Material.h"
#include "Device.h"
#include "Sphere.h"

Cube::Cube(wstring file)
{
	_material = new Material(file);
	_material->SetDiffuseMap(L"../Assets/Textures/WallDiffuse.png");

	Create();
}

Cube::~Cube()
{
	delete _mesh;
	delete _material;
}

void Cube::Update()
{

}

void Cube::Render()
{
	_mesh->IASet();
	_material->Set();
	SetWorldBuffer();

	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void Cube::PostRender()
{

}

void Cube::Create()
{
	_vertices.resize(24);
	//Front
	_vertices[0].position = { -1, -1, -1 };
	_vertices[1].position = { -1, +1, -1 };
	_vertices[2].position = { +1, +1, -1 };
	_vertices[3].position = { +1, -1, -1 };

	//Back
	_vertices[4].position = { -1, -1, +1 };
	_vertices[5].position = { -1, +1, +1 };
	_vertices[6].position = { +1, +1, +1 };
	_vertices[7].position = { +1, -1, +1 };

	//Right
	_vertices[8].position = { +1, -1, -1 };
	_vertices[9].position = { +1, +1, -1 };
	_vertices[10].position = { +1, +1, +1 };
	_vertices[11].position = { +1, -1, +1 };

	//Left
	_vertices[12].position = { -1, -1, -1 };
	_vertices[13].position = { -1, +1, -1 };
	_vertices[14].position = { -1, +1, +1 };
	_vertices[15].position = { -1, -1, +1 };

	//Up
	_vertices[16].position = { -1, +1, -1 };
	_vertices[17].position = { -1, +1, +1 };
	_vertices[18].position = { +1, +1, +1 };
	_vertices[19].position = { +1, +1, -1 };

	//Down
	_vertices[20].position = { -1, -1, -1 };
	_vertices[21].position = { -1, -1, +1 };
	_vertices[22].position = { +1, -1, +1 };
	_vertices[23].position = { +1, -1, -1 };

	for (UINT i = 0; i < 6; i++)
	{
		_vertices[i * 4 + 0].uv = { 0, 1 };
		_vertices[i * 4 + 1].uv = { 0, 0 };
		_vertices[i * 4 + 2].uv = { 1, 0 };
		_vertices[i * 4 + 3].uv = { 1, 1 };
	}

	//Front
	_indices.emplace_back(0);
	_indices.emplace_back(1);
	_indices.emplace_back(2);

	_indices.emplace_back(0);
	_indices.emplace_back(2);
	_indices.emplace_back(3);

	//Back
	_indices.emplace_back(4);
	_indices.emplace_back(6);
	_indices.emplace_back(5);

	_indices.emplace_back(4);
	_indices.emplace_back(7);
	_indices.emplace_back(6);

	//Right
	_indices.emplace_back(8);
	_indices.emplace_back(9);
	_indices.emplace_back(10);

	_indices.emplace_back(8);
	_indices.emplace_back(10);
	_indices.emplace_back(11);

	//Left
	_indices.emplace_back(12);
	_indices.emplace_back(14);
	_indices.emplace_back(13);

	_indices.emplace_back(12);
	_indices.emplace_back(15);
	_indices.emplace_back(14);

	//Up
	_indices.emplace_back(16);
	_indices.emplace_back(17);
	_indices.emplace_back(18);

	_indices.emplace_back(16);
	_indices.emplace_back(18);
	_indices.emplace_back(19);

	//Down
	_indices.emplace_back(20);
	_indices.emplace_back(22);
	_indices.emplace_back(21);

	_indices.emplace_back(20);
	_indices.emplace_back(23);
	_indices.emplace_back(22);

	CreateNormal();

	_mesh = new Mesh(_vertices.data(), sizeof(VertexUVNormal), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

void Cube::CreateNormal()
{
	for (UINT i = 0; i < _indices.size() / 3; i++)
	{
		UINT index0 = _indices[i * 3 + 0];
		UINT index1 = _indices[i * 3 + 1];
		UINT index2 = _indices[i * 3 + 2];

		Vector3 v0 = _vertices[index0].position;
		Vector3 v1 = _vertices[index1].position;
		Vector3 v2 = _vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = A.Cross(B);
		normal.Normalize();

		_vertices[index0].normal = normal;
		_vertices[index1].normal = normal;
		_vertices[index2].normal = normal;
	}
}
