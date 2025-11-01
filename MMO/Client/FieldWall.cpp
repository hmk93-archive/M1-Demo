#include "pch.h"
#include "FieldWall.h"
#include "Material.h"
#include "Mesh.h"
#include "Device.h"

FieldWall::FieldWall(Vector2 size, Vector2 layout, wstring textureFile)
	: _size(size)
	, _layout(layout)
{
	CreateMesh();
	CreateMaterial(textureFile);
}

FieldWall::~FieldWall()
{
	delete _material;
	delete _mesh;
}

void FieldWall::Update()
{
	if (!isActive)
		return;

	Transform::UpdateWorld();
}

void FieldWall::Render()
{
	if (!isActive)
		return;

	_mesh->IASet();
	SetWorldBuffer();
	_material->Set();

	Device::Get().GetDeviceContext()->DrawIndexed(_indices.size(), 0, 0);
}

void FieldWall::CreateMesh()
{
	VertexType v = {};
	v.position = { 0, _size.y, 0 };
	v.uv = { 0, 0 };
	_vertices.emplace_back(v);

	v.position = { _size.x, _size.y, 0 };
	v.uv = { _layout.x, 0 };
	_vertices.emplace_back(v);

	v.position = { 0, 0, 0 };
	v.uv = { 0, _layout.y };
	_vertices.emplace_back(v);

	v.position = { _size.x, 0, 0 };
	v.uv = { _layout.x, _layout.y };
	_vertices.emplace_back(v);

	_indices.resize(6);
	_indices = 
	{
		0, 1, 3,
		0, 3, 2
	};

	_mesh = new Mesh(_vertices.data(), sizeof(VertexType), _vertices.size(), _indices.data(), _indices.size());
}

void FieldWall::CreateMaterial(wstring textureFile)
{
	_material = new Material(L"Default");
	
	wstring file;
	file = L"../Assets/Textures/" + textureFile + L"Diffuse" + L".png";
	_material->SetDiffuseMap(file);
}
