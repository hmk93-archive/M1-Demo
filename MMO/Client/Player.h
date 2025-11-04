#pragma once

#include "ModelAnimator.h"
#include "Transform.h"

class Terrain;
class Collider;
class AStar;
class NavMesh;

class Player : public ModelAnimator, public Transform
{
public:
	enum PlayerAnimState
	{
		Idle,
		Run,
		Jump,
		Attack_A,
		Attack_B,
		End,
	} animState = Idle;

	enum PlayerBehaviourState
	{
		None,
		War,
		Air,
	} behaviourState = None;

public:
	Player(string file);
	~Player();

	void Update();

	void Render();
	void PostRender();

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }
	void SetAStar(AStar* astar) { _astar = astar; }
	void SetNavMesh(NavMesh* navMesh) { _navMesh = navMesh; }

	Collider* GetMainCollider() { return _mainCollider; }
	Collider* GetEventCollider() { return _eventCollider; }

	void Attack();
	void PushBack(Collider* other);
	void LookAt(Vector3 direction);

private:
	void CreateCollider();

	void UpdateMatrix();

	void Control();
	void Move();
	void Rotate();

	void SetIdle();
	void Combo();
	void AttackEnd();

	void SetPath();
	void SetVelocity();
	void SetHeight();
	void SetAnimation(PlayerAnimState value, float speed = 1.0f);

private:
	Terrain* _terrain = nullptr;
	AStar* _astar = nullptr;
	NavMesh* _navMesh = nullptr;

	Vector3 _destPos = Vector3(0.0f);

	Vector3 _velocity = Vector3(0.0f);

	float _moveSpeed = 20.0f;
	float _rotateSpeed = 20.0f;
	float _deceleration = 5.0f;

	Collider* _mainCollider = nullptr;
	Collider* _eventCollider = nullptr;

	Matrix _body = Matrix::Identity;

	vector<Vector3> _path = {};

	bool _isCombo = false;
};

