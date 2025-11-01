#pragma once

#include "Collider.h"

class UICollider : public Collider
{
public:
	UICollider(Vector4 box);
	virtual ~UICollider();

	virtual bool MouseCollision() override;
	virtual void SetSize(Vector4 value) override;

private:
	virtual void CreateMesh() override;
	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;

private:
	Vector4 _box = {};
};