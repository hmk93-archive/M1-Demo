#pragma once

#include "ModelAnimators.h"

class Terrain;
class Collider;
class Player;
class HUDHPBar;

class Enemy : public ModelAnimators
{
public:
	enum EnemyAnimState
	{
		Idle,
		Run,
		Punch,
		Hit,
		Dead,
	} state = Idle;

	enum EnemyBehaviourState
	{
		None,
		War,
		Jump,
		Die,
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
	void SetHP(UINT instanceID, int damage);

	HUDHPBar* AddHPBar();

	void Death(UINT instanceID);
	void DeathEnd(int instanceID);

protected:
	void CheckDistance(UINT instanceID);
	void RotateTo(UINT instanceID);
	void MoveTo(UINT instanceID);

public:
	bool onMouse = false;

public:
	Collider* mainCollider[MAX_INSTANCE] = {};
	HUDHPBar* hpBar[MAX_INSTANCE] = {};

protected:
	float _attackRange = 25.0f;
	float _moveSpeed = 10.0f;

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;

	int _curHP = 100;
	int _maxHP = 100;
};

