#include "pch.h"
#include "UICollider.h"
#include "Control.h"
#include "Mesh.h"

UICollider::UICollider(Vector4 box)
    :_box(box)
{
    _type = Sqaure;
    CreateMesh();
}

UICollider::~UICollider()
{
}

bool UICollider::MouseCollision()
{
	UpdateWorld();

	Vector3 p = Control::Get().GetMouse();
	p.y = g_screenHeight - p.y;
    float l = _box.x + position.x;
	float r = _box.z + position.x;
	float t = _box.y + position.y;
	float b = _box.w + position.y;
	if (p.x < l || p.x > r || p.y < b || p.y > t)
		return false;
    else
		return true;
}

void UICollider::SetSize(Vector4 value)
{
    _vertices.clear();
    _box = value;
    float l = _box.x;
    float t = _box.y;
    float r = _box.z;
    float b = _box.w;
    _vertices.emplace_back(l, b, 0);
    _vertices.emplace_back(l, t, 0);
    _vertices.emplace_back(r, t, 0);
    _vertices.emplace_back(r, b, 0);
    _mesh->UpdateVertex(_vertices.data(), (UINT)_vertices.size());
}

void UICollider::CreateMesh()
{
    float l = _box.x;
    float t = _box.y;
    float r = _box.z;
    float b = _box.w;
    _vertices.emplace_back(l, b, 0);
    _vertices.emplace_back(l, t, 0);
    _vertices.emplace_back(r, t, 0);
    _vertices.emplace_back(r, b, 0);
	_indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
    _mesh = new Mesh(_vertices.data(), sizeof(Vertex), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

bool UICollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    return false;
}

bool UICollider::BoxCollision(BoxCollider* collider)
{
    return false;
}

bool UICollider::SphereCollision(SphereCollider* collider)
{
    return false;
}

bool UICollider::CapsuleCollision(CapsuleCollider* collider)
{
    return false;
}
