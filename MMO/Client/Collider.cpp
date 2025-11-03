#include "pch.h"
#include "Collider.h"
#include "Material.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Device.h"
#include "Mesh.h"

bool Collider::s_isColliderDraw = true;

Collider::Collider()
{
    _material = new Material(L"Collider");
    _material->GetBuffer()->data.diffuse = Vector4(0, 1, 0, 1);
}

Collider::~Collider()
{
    delete _material;
    delete _mesh;
}

bool Collider::Collision(Collider* collider)
{
    UpdateWorld();
    collider->UpdateWorld();

	switch (collider->_type)
	{
	case Collider::Box:
		return BoxCollision(static_cast<BoxCollider*>(collider));
	case Collider::Sphere:
		return SphereCollision(static_cast<SphereCollider*>(collider));
	case Collider::Capsule:
		return CapsuleCollision(static_cast<CapsuleCollider*>(collider));
	case Collider::Square:
		return MouseCollision();
	}

	return false;
}

void Collider::Render()
{
    if (isActive == false)
        return;

    SetWorldBuffer();

	if (s_isColliderDraw == false)
        return;

    _mesh->IASet(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    _material->Set();

    Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);

    //RenderAxis();
}

void Collider::SetColor(Vector4 color) 
{
    _material->GetBuffer()->data.diffuse = color; 
}
