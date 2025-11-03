#include "pch.h"
#include "Transform.h"
#include "GlobalBuffers.h"
#include "Mesh.h"
#include "Material.h"
#include "Device.h"
#include "Utility.h"
using namespace Utility;

Transform::Transform(string name)
{
	tag = name;
	_worldBuffer = new MatrixBuffer();
	_transformBuffer = new MatrixBuffer();
	_material = new Material(L"Transform");

	CreateAxis();
}

Transform::~Transform()
{
	delete _mesh;
	delete _material;
	delete _transformBuffer;
	delete _worldBuffer;
}

void Transform::UpdateWorld()
{
	if (_target)
		position = _target->GlobalPos() + _targetOffset;

	_world = Matrix::CreateScale(scale);
	_world *= Matrix::CreateRotationX(rotation.x);
	_world *= Matrix::CreateRotationY(rotation.y);
	_world *= Matrix::CreateRotationZ(rotation.z);
	_world *= Matrix::CreateTranslation(position);

	if (_parent)
		_world *= *_parent;

	Quaternion q;
	_world.Decompose(_globalScale, q, _globalPosition);
	_globalRotation = ToEulerAngles(q);

	_worldBuffer->Set(_world);
}

void Transform::SetWorldBuffer(UINT slot)
{
	UpdateWorld();
	_worldBuffer->SetVSBuffer(slot);
}

void Transform::SetTarget(Transform* target, Vector3 offset)
{
	_target = target;
	_targetOffset = offset;
}

void Transform::RenderAxis()
{
	Matrix matrix = Matrix::CreateScale(1.0f);
	matrix *= Matrix::CreateRotationX(_globalRotation.x);
	matrix *= Matrix::CreateRotationY(_globalRotation.y);
	matrix *= Matrix::CreateRotationZ(_globalRotation.z);
	matrix *= Matrix::CreateTranslation(_globalPosition);

	_transformBuffer->Set(matrix);
	_transformBuffer->SetVSBuffer(0);

	_mesh->IASet();
	_material->Set();

	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void Transform::CreateAxis()
{
	float length = 3.0f;
	float thickness = 0.5f;

	//Axis X		
	Vector4 color = { 1, 0, 0, 1 };
	_vertices.emplace_back(Vector3(0, 0, 0), color);
	_vertices.emplace_back(Vector3(length, 0, 0), color);
	_vertices.emplace_back(Vector3(length, thickness, 0), color);
	_vertices.emplace_back(Vector3(length, 0, thickness), color);
	_indices.emplace_back(0);
	_indices.emplace_back(2);
	_indices.emplace_back(1);
	_indices.emplace_back(0);
	_indices.emplace_back(1);
	_indices.emplace_back(3);
	_indices.emplace_back(0);
	_indices.emplace_back(3);
	_indices.emplace_back(2);
	_indices.emplace_back(1);
	_indices.emplace_back(2);
	_indices.emplace_back(3);

	//Axis Y
	color = { 0, 1, 0, 1 };
	_vertices.emplace_back(Vector3(0, 0, 0), color);
	_vertices.emplace_back(Vector3(0, length, 0), color);
	_vertices.emplace_back(Vector3(0, length, thickness), color);
	_vertices.emplace_back(Vector3(thickness, length, 0), color);
	_indices.emplace_back(4);
	_indices.emplace_back(6);
	_indices.emplace_back(5);
	_indices.emplace_back(4);
	_indices.emplace_back(5);
	_indices.emplace_back(7);
	_indices.emplace_back(4);
	_indices.emplace_back(7);
	_indices.emplace_back(6);
	_indices.emplace_back(5);
	_indices.emplace_back(6);
	_indices.emplace_back(7);

	//Axis Z
	color = { 0, 0, 1, 1 };
	_vertices.emplace_back(Vector3(0, 0, 0), color);
	_vertices.emplace_back(Vector3(0, 0, length), color);
	_vertices.emplace_back(Vector3(thickness, 0, length), color);
	_vertices.emplace_back(Vector3(0, thickness, length), color);
	_indices.emplace_back(8);
	_indices.emplace_back(10);
	_indices.emplace_back(9);
	_indices.emplace_back(8);
	_indices.emplace_back(9);
	_indices.emplace_back(11);
	_indices.emplace_back(8);
	_indices.emplace_back(11);
	_indices.emplace_back(10);
	_indices.emplace_back(9);
	_indices.emplace_back(10);
	_indices.emplace_back(11);

	_mesh = new Mesh(_vertices.data(), sizeof(VertexColor), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}
