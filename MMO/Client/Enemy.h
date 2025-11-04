#pragma once

#include "ModelAnimators.h"

class Terrain;
class Collider;
class Player;

class Enemy : public ModelAnimators
{
public:
	enum EnemyAnimState
	{
		Idle,
		Run,
		Punch,
		Hit,
	} state = Idle;

	enum EnemyBehaviourState
	{
		None,
		War,
		Jump,
	} behaviourState = None;

public:
	Enemy(string file);
	virtual ~Enemy();

	void Update();
	void Render();
	void PostRender();

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }

	void SetPlayer(Player* player) { _player = player; }

	void Damage(UINT instanceID, UINT damage);
	void UpdateAI(UINT instanceID);
	void SetAnimation(UINT instanceID, EnemyAnimState value, float speed = 1.0f);

	void SetIdle(int instanceID);

protected:
	void CheckDistance(UINT instanceID);
	void RotateTo(UINT instanceID);
	void MoveTo(UINT instanceID);

public:
	bool onMouse = false;

public:
	Collider* mainCollider[MAX_INSTANCE] = {};

protected:
	float _attackRange = 25.0f;
	float _moveSpeed = 10.0f;

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
};

