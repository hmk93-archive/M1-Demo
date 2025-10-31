#include "pch.h"
#include "UICollider.h"
#include "Control.h"
#include "Mesh.h"

UICollider::UICollider(Vector4 box)
    :box(box)
{
    type = UI;
    CreateMesh();
}

UICollider::~UICollider()
{
}

bool UICollider::MouseCollision()
{
	UpdateWorld();
    //
	Vector3 p = Control::Get().GetMouse();
	p.y = g_screenHeight - p.y;
    float l = box.x + position.x;
	float r = box.z + position.x;
	float t = box.y + position.y;
	float b = box.w + position.y;
	if (p.x < l || p.x > r || p.y < b || p.y > t)
		return false;
    else
		return true;
}

void UICollider::SetSize(Vector4 value)
{
    vertices.clear();
    //
    box = value;
    //
    float l = box.x;
    float t = box.y;
    float r = box.z;
    float b = box.w;
    vertices.emplace_back(l, b, 0);
    vertices.emplace_back(l, t, 0);
    vertices.emplace_back(r, t, 0);
    vertices.emplace_back(r, b, 0);
    //
    mesh->UpdateVertex(vertices.data(), vertices.size());
}

void UICollider::CreateMesh()
{
    float l = box.x;
    float t = box.y;
    float r = box.z;
    float b = box.w;
    vertices.emplace_back(l, b, 0);
    vertices.emplace_back(l, t, 0);
    vertices.emplace_back(r, t, 0);
    vertices.emplace_back(r, b, 0);
    //
	indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
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
