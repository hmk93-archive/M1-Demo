#include "pch.h"
#include "SquareCollider.h"
#include "Input.h"
#include "Mesh.h"

SquareCollider::SquareCollider(Vector4 square)
    :_square(square)
{
    _type = Square;
    CreateMesh();
}

SquareCollider::~SquareCollider()
{
}

bool SquareCollider::MouseCollision()
{
	UpdateWorld();

	Vector3 p = Input::Get().GetMouse();
	p.y = g_screenHeight - p.y;
    float l = _square.x + position.x;
	float r = _square.z + position.x;
	float t = _square.y + position.y;
	float b = _square.w + position.y;
	if (p.x < l || p.x > r || p.y < b || p.y > t)
		return false;
    else
		return true;
}

void SquareCollider::SetSize(Vector4 value)
{
    _vertices.clear();
    _square = value;
    float l = _square.x;
    float t = _square.y;
    float r = _square.z;
    float b = _square.w;
    _vertices.emplace_back(l, b, 0);
    _vertices.emplace_back(l, t, 0);
    _vertices.emplace_back(r, t, 0);
    _vertices.emplace_back(r, b, 0);
    _mesh->UpdateVertex(_vertices.data(), (UINT)_vertices.size());
}

void SquareCollider::CreateMesh()
{
    float l = _square.x;
    float t = _square.y;
    float r = _square.z;
    float b = _square.w;
    _vertices.emplace_back(l, b, 0);
    _vertices.emplace_back(l, t, 0);
    _vertices.emplace_back(r, t, 0);
    _vertices.emplace_back(r, b, 0);
	_indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
    _mesh = new Mesh(_vertices.data(), sizeof(Vertex), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

bool SquareCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    return false;
}

bool SquareCollider::BoxCollision(BoxCollider* collider)
{
    return false;
}

bool SquareCollider::SphereCollision(SphereCollider* collider)
{
    return false;
}

bool SquareCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return false;
}
