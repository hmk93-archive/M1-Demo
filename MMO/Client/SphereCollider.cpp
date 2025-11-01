#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "Mesh.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    : _radius(radius)
	, _stackCount(stackCount)
	, _sliceCount(sliceCount)
{
    _type = Sphere;
    CreateMesh();
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
	Vector3 P = ray.position;
	Vector3 D = ray.direction;

	Vector3 C = GlobalPos();
	Vector3 A = P - C;

	float a = D.Dot(D);
	float b = 2.0f * D.Dot(A);
	float c = A.Dot(A) - Radius() * Radius();

	if (b * b >= 4 * a * c)
	{
		if (contact != nullptr)
		{
			float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			contact->distance = t;
			contact->hitPoint = P + D * t;
		}

		return true;
	}

    return false;
}

bool SphereCollider::BoxCollision(BoxCollider* collider)
{
    return collider->SphereCollision(this);
}

bool SphereCollider::SphereCollision(SphereCollider* collider)
{
	float distance = (GlobalPos(), collider->GlobalPos()).Length();

	float r1 = Radius();
	float r2 = collider->Radius();

    return distance <= r1 + r2;
}

bool SphereCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return collider->SphereCollision(this);
}

void SphereCollider::CreateMesh()
{
	float phiStep = XM_PI / _stackCount;
	float thetaStep = XM_2PI / _sliceCount;

	for (UINT i = 0; i <= _stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= _sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex;

			vertex.position.x = sin(phi) * cos(theta) * _radius;
			vertex.position.y = cos(phi) * _radius;
			vertex.position.z = sin(phi) * sin(theta) * _radius;

			_vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < _stackCount; i++)
	{
		for (UINT j = 0; j < _sliceCount; j++)
		{
			_indices.emplace_back((_sliceCount + 1) * i + j);
			_indices.emplace_back((_sliceCount + 1) * i + j + 1);			

			_indices.emplace_back((_sliceCount + 1) * i + j);
			_indices.emplace_back((_sliceCount + 1) * (i + 1) + j);
		}
	}

	_mesh = new Mesh(_vertices.data(), sizeof(Vertex), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}
