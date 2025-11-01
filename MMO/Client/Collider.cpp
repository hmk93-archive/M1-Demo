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
    material = new Material(L"Collider");
    material->GetBuffer()->data.diffuse = Vector4(0, 1, 0, 1);
}

Collider::~Collider()
{
    delete material;
    delete mesh;
}

bool Collider::Collision(Collider* collider)
{
    UpdateWorld();
    collider->UpdateWorld();
    //
	switch (collider->type)
	{
	case Collider::BOX:
		return BoxCollision(static_cast<BoxCollider*>(collider));
	case Collider::SPHERE:
		return SphereCollision(static_cast<SphereCollider*>(collider));
	case Collider::CAPSULE:
		return CapsuleCollision(static_cast<CapsuleCollider*>(collider));
	case Collider::UI:
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

    mesh->IASet(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    material->Set();

    Device::Get().GetDeviceContext()->DrawIndexed(indices.size(), 0, 0);

    //RenderAxis();
}

void Collider::SetColor(Vector4 color) 
{
    material->GetBuffer()->data.diffuse = color; 
}
