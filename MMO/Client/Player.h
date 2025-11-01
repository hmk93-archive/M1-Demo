#pragma once

#include "ModelAnimator.h"
#include "Transform.h"

class Terrain;
class Collider;

class Player : public ModelAnimator, public Transform
{
	enum PlayerAnimState
	{
		Idle,
		Run,
		Attack_A,
		End
	} state;
public:
	Player(string file);
	~Player();

	void Update();

	void Render();
	void PostRender();

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }

	Collider* GetMainCollider() { return _mainCollider; }

	void Attack();

private:
	void CreateCollider();

	void UpdateMatrix();

	void Control();
	void Move();
	void Rotate();

	void SetVelocity();
	void SetHeight();
	void SetAnimation(PlayerAnimState value, float speed = 1.0f);

	// TMP
public:
	Vector3 _destPos = Vector3(0.0f);

private:
	Terrain* _terrain = nullptr;

	Vector3 _velocity = Vector3(0.0f);

	float _moveSpeed = 20.0f;
	float _rotateSpeed = 20.0f;
	float _deceleration = 10.0f;

	Collider* _mainCollider = nullptr;

	Matrix _body = Matrix::Identity;
};

