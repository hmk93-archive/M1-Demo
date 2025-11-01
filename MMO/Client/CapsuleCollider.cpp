#include "pch.h"
#include "CapsuleCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Mesh.h"
#include "Utility.h"
using namespace Utility;

CapsuleCollider::CapsuleCollider(float radius, float height, UINT stackCount, UINT sliceCount)
    : _radius(radius)
	, _height(height)
	, _stackCount(stackCount)
	, _sliceCount(sliceCount)
{
    _type = Capsule;
    CreateMesh();
}

CapsuleCollider::~CapsuleCollider()
{
}

bool CapsuleCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
	Vector3 direction = Up();

	Vector3 pa = GlobalPos() - direction * Height() * 0.5f;
	Vector3 pb = GlobalPos() + direction * Height() * 0.5f;

	Vector3 ro = ray.position;
	Vector3 rd = ray.direction;

	float r = Radius();

	Vector3 ba = pb - pa;
	Vector3 oa = ro - pa;

	float baba = ba.Dot(ba);
	float bard = ba.Dot(rd);
	float baoa = ba.Dot(oa);
	float rdoa = rd.Dot(oa);
	float oaoa = oa.Dot(oa);

	float a = baba - bard * bard;
	float b = baba * rdoa - baoa * bard;
	float c = baba * oaoa - baoa * baoa - r * r * baba;
	float h = b * b - a * c;

	if (h >= 0.0f)
	{
		float t = (-b - sqrt(h)) / a;

		float y = baoa + t * bard;

		if (y > 0.0f && y < baba)//Body
		{
			if (contact != nullptr)
			{
				contact->distance = t;
				contact->hitPoint = ray.position + ray.direction * t;
			}			
			return true;
		}

		Vector3 oc = (y <= 0.0f) ? oa : ro - pb;
		b = rd.Dot(oc);
		c = oc.Dot(oc) - r * r;
		h = b * b - c;
		if (h > 0.0f)
		{
			if (contact != nullptr)
			{
				contact->distance = -b - sqrt(h);
				contact->hitPoint = ray.position + ray.direction * contact->distance;
			}
			return true;
		}
	}

    return false;
}

bool CapsuleCollider::BoxCollision(BoxCollider* collider)
{
	Vector3 direction = Up();
	Vector3 startPos = GlobalPos() - direction * Height() * 0.5f;

	Vector3 A = collider->GlobalPos() - startPos;

	float t = A.Dot(direction);
	t = max(0.0f, t);
	t = min(Height(), t);

	Vector3 pointOnLine = startPos + direction * t;	

	return collider->SphereCollision(pointOnLine, Radius());
}

bool CapsuleCollider::SphereCollision(SphereCollider* collider)
{
	Vector3 direction = Up();
	Vector3 startPos = GlobalPos() - direction * Height() * 0.5f;

	Vector3 A = collider->GlobalPos() - startPos;

	float t = A.Dot(direction);
	t = max(0.0f, t);
	t = min(Height(), t);

	Vector3 pointOnLine = startPos + direction * t;

	float distance = (pointOnLine - collider->GlobalPos()).Length();

    return distance <= (Radius() + collider->Radius());
}

bool CapsuleCollider::CapsuleCollision(CapsuleCollider* collider)
{
	Vector3 aDirection = Up();

	Vector3 aA = GlobalPos() - aDirection * Height() * 0.5f;
	Vector3 aB = GlobalPos() + aDirection * Height() * 0.5f;

	Vector3 bDirection = collider->Up();

	Vector3 bA = collider->GlobalPos() - bDirection * collider->Height() * 0.5f;
	Vector3 bB = collider->GlobalPos() + bDirection * collider->Height() * 0.5f;

	Vector3 v0 = bA - aA;
	Vector3 v1 = bB - aA;
	Vector3 v2 = bA - aB;
	Vector3 v3 = bB - aB;

	float d0 = v0.Dot(v0);
	float d1 = v1.Dot(v1);
	float d2 = v2.Dot(v2);
	float d3 = v3.Dot(v3);

	Vector3 bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
		bestA = aB;
	else
		bestA = aA;

	Vector3 bestB = ClosestPointOnLineSegment(bA, bB, bestA);
	bestA = ClosestPointOnLineSegment(aA, aB, bestB);

	float distance = (bestA - bestB).Length();

    return distance <= (Radius() + collider->Radius());
}

void CapsuleCollider::CreateMesh()
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

			if (vertex.position.y > 0)
				vertex.position.y += _height * 0.5f;
			else
				vertex.position.y -= _height * 0.5f;

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
