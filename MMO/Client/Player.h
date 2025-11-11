#pragma once

#include "ModelAnimator.h"
#include "Transform.h"

class Terrain;
class Collider;
class AStar;
class NavMesh;
class Cursor;
class ModelObject;
class RasterizerState;

class Player : public ModelAnimator, public Transform
{
public:
	enum PlayerAnimState
	{
		Idle,
		RunF,
		RunL,
		RunR,
		RunB,
		Jump,
		Attack_A,
		Attack_B,
		Impact,
		Dead,
		End,
	} animState = Idle;

	enum PlayerBehaviourState
	{
		None,
		War,
		Air,
		Die,
	} behaviourState = None;

public:
	Player(string file);
	~Player();

	void Update();

	void Render();
	void PostRender();

	void SetTerrain(Terrain* terrain);
	void SetAStar(AStar* astar) { _astar = astar; }
	void SetNavMesh(NavMesh* navMesh) { _navMesh = navMesh; }

	Collider* GetMainCollider() { return _mainCollider; }
	Collider* GetEventCollider() { return _eventCollider; }
	Collider* GetSwordCollider();

	void Attack(int type);
	void PushBack(Collider* other);
	void LookAt(Vector3 direction);

	int GetAttackDamage() { return _attackDamage; }

	void Hit(int damage);

	void Death();

private:
	void CreateCollider();
	void CreateWeapons();
	void CreateCursor();

	void UpdateMatrix();

	void Control();
	void Move();
	void Rotate();

	void SetIdle();
	void AttackEnd();

	void ActiveWeaponCollider();

	void SetPath();
	void SetVelocity();
	void SetHeight();
	void SetAnimation(PlayerAnimState value, float speed = 1.0f);

	void SetClips(string file);

private:
	Terrain* _terrain = nullptr;
	AStar* _astar = nullptr;
	NavMesh* _navMesh = nullptr;

	Vector3 _destPos = Vector3(0.0f);

	Vector3 _velocity = Vector3(0.0f);

	float _moveSpeed = 20.0f;
	float _rotateSpeed = 20.0f;
	float _deceleration = 20.0f;

	Collider* _mainCollider = nullptr;
	Collider* _eventCollider = nullptr;

	Matrix _body = Matrix::Identity;
	Matrix _rightHand = Matrix::Identity;

	vector<Vector3> _path = {};

	bool _isCombo = false;

	Cursor* _cursor = nullptr;

	int _attackDamage = 20;

	ModelObject* _sword = nullptr;

	int _curHP = 100;
	int _maxHP = 100;

	RasterizerState* _rsState[2] = {};
};

