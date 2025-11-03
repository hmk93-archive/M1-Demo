#pragma once

#include "Transform.h"

struct Contact
{
	Vector3 hitPoint;
	float distance;
};

class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class SquareCollider;
class Material;
class Mesh;

class Collider : public Transform
{
public:
	static bool s_isColliderDraw;

public:
	enum Type
	{
		Box,
		Sphere,
		Capsule,
		Square,
		None
	};

public:
	Collider();
	virtual ~Collider();

	bool Collision(Collider* collider);
	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;
	virtual bool BoxCollision(BoxCollider* collider) = 0;
	virtual bool SphereCollision(SphereCollider* collider) = 0;
	virtual bool CapsuleCollision(CapsuleCollider* collider) = 0;
	virtual bool MouseCollision() { return 0; };

	virtual void CreateMesh() = 0;

	void Render();

	void SetColor(Vector4 color);

	virtual void SetSize(Vector4 value) {};

protected:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	Type _type = Type::None;

	vector<Vertex> _vertices = {};
	vector<UINT> _indices = {};
};