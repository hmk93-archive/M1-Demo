#include "pch.h"
#include "FieldWall.h"
#include "BoxCollider.h"
#include "Mesh.h"
#include "Material.h"
#include "Device.h"

FieldWall::FieldWall(Vector2 size, Vector2 layout, wstring textureFile)
	: _size(size)
	, _layout(layout)
{
	CreateMaterial(textureFile);
	CreateMesh();
	//
	// rsState[0] = new RasterizerState();
	// rsState[1] = new RasterizerState();
	// rsState[1]->CullMode(D3D11_CULL_NONE);
	//
	float halfDepth = 1.5f;
	Vector3 minBox = { 0, 0, -halfDepth };
	Vector3 maxBox = { size.x, size.y, halfDepth };
	_collider = new BoxCollider(minBox, maxBox);
	_collider->SetParent(GetWorld());
}

FieldWall::~FieldWall()
{
	delete _collider;
	//
	// delete rsState[0];
	// delete rsState[1];
	//
	delete _material;
	delete _mesh;
}

void FieldWall::Update()
{
	if (isActive == false) return;
	//
	Transform::UpdateWorld();
}

void FieldWall::Render()
{
	if (isActive == false) return;
	//
	_mesh->IASet();
	SetWorldBuffer();
	_material->Set();
	//
	// rsState[1]->SetState();
	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
	// rsState[0]->SetState();
	//
	Transform::RenderAxis();
	//
	_collider->Render();
}

void FieldWall::CreateMesh()
{
	// Vertex
	VertexType v{};

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
	_indices = {
		0, 1, 3,
		0, 3, 2
	};

	_mesh = new Mesh(_vertices.data(), sizeof(VertexType), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

void FieldWall::CreateMaterial(wstring textureFile)
{
	_material = new Material(L"Default");

	wstring filePath{};
	filePath = L"../Assets/Textures/" + textureFile + L"Diffuse" + L".png";
	_material->SetDiffuseMap(filePath);
	filePath = L"../Assets/Textures/" + textureFile + L"Normal" + L".png";
	_material->SetNormalMap(filePath);
}