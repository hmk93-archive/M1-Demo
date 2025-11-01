#pragma once

#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(float radius = 1.0f, float height = 2.0f, UINT stackCount = 15, UINT sliceCount = 30);
	~CapsuleCollider();

	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return _radius * max(GlobalScale().x, max(GlobalScale().y, GlobalScale().z)); }
	float Height() { return _height * GlobalScale().y; }

private:
	virtual void CreateMesh() override;

private:
	float _radius = 1.0f;
	float _height = 2.0f;

	UINT _stackCount = 15;
	UINT _sliceCount = 20;
};