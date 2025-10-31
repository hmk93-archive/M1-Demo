#pragma once

#include "Collider.h"

class UICollider : public Collider
{
private:
	Vector4 box;
	//
public:
	UICollider(Vector4 box);
	virtual ~UICollider();
	//
	virtual bool MouseCollision() override;
	virtual void SetSize(Vector4 value) override;
	//
private:
	virtual void CreateMesh() override;
	// Collider��(��) ���� ��ӵ�
	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;
};