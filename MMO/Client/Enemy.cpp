#include "pch.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "Player.h"
#include "Timer.h"
#include "Utility.h"
#include "HUDHPBar.h"
#include "FloatText.h"
#include "UIManager.h"
using namespace Utility;

Enemy::Enemy(string file)
	: ModelAnimators(file + "/" + file)
{
	SetShader(L"Instancing");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	ReadClip(file + "/Punch0");
	ReadClip(file + "/Hit0");
	ReadClip(file + "/Dead0");

	for (UINT i = 0; i < MAX_INSTANCE; i++)
		curHP[i] = maxHP[i] = 100;
}

Enemy::~Enemy()
{
	for (HUDHPBar* hp : hpBar)
		delete hp;
	for(Collider* collider : mainCollider)
		delete collider;
}

void Enemy::Update()
{
	for (UINT i = 0; i < _transforms.size(); i++)
	{
		mainCollider[i]->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		hpBar[i]->Update();
		hpBar[i]->SetPos(_transforms[i]->position);
	}

	ModelAnimators::Update();
}

void Enemy::Render()
{
	for (UINT i = 0; i < _transforms.size(); i++)
		_transforms[i]->SetWorldBuffer();

	ModelAnimators::Render();

	for (UINT i = 0; i < _transforms.size(); i++)
		mainCollider[i]->Render();
}

void Enemy::PostRender()
{
	for (UINT i = 0; i < _drawCount; i++)
		hpBar[i]->Render();	
}

void Enemy::Damage(UINT instanceID, UINT damage)
{
	if (behaviourState[instanceID] == Die)
		return;

	mainCollider[instanceID]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	SetAnimation(instanceID, EnemyAnimState::Hit);

	SetHP(instanceID, damage);
}

void Enemy::CheckDistance(UINT instanceID)
{
	if (!_player)
		return;

	if (behaviourState[instanceID] == War)
		return;

	const float distance = (_player->position - _transforms[instanceID]->position).Length();
	if (distance > _followRange)
	{
		SetAnimation(instanceID, Idle);
		behaviourState[instanceID] = None;
	}
	else
	{
		if (distance > _attackRange)
		{
			SetAnimation(instanceID, Run);
			behaviourState[instanceID] = Move;
		}
		else
		{
			SetAnimation(instanceID, Punch);
			behaviourState[instanceID] = War;
		}
	}
}

void Enemy::RotateTo(UINT instanceID)
{
	if (!_player)
		return;
	Vector3 direction = (_player->position - _transforms[instanceID]->position);
	direction.Normalize();
	LookAt(_transforms[instanceID]->rotation.y, direction);
}

void Enemy::SetIdle(int instanceID)
{
	if (behaviourState[instanceID] == Die)
	{
		_transforms[instanceID]->isActive = true;
		mainCollider[instanceID]->isActive = true;
		hpBar[instanceID]->isActive = true;
	}

	behaviourState[instanceID] = None;
	SetAnimation(instanceID, Idle);
}

void Enemy::SetHP(UINT instanceID, int damage)
{
	UIManager::Get().InitFloatText(_transforms[instanceID], ToWString(damage));

	curHP[instanceID] -= damage;
	if (curHP[instanceID] <= 0)
		Death(instanceID);

	float hpRatio = (float)curHP[instanceID] / (float)maxHP[instanceID];
	hpBar[instanceID]->isHit = true;
	hpBar[instanceID]->SetScale(HUDHPBar::Face, hpRatio);
	hpBar[instanceID]->SetCurrentBodyScale();
}

HUDHPBar* Enemy::AddHPBar(UINT instanceID)
{
	HUDHPBar* bar = new HUDHPBar();
	hpBar[instanceID] = bar;
	return bar;
}

void Enemy::OnAttack()
{
	int damage = 10;
	_player->Hit(damage);
}

void Enemy::Death(UINT instanceID)
{
	curHP[instanceID] = 0;

	mainCollider[instanceID]->isActive = false;
	hpBar[instanceID]->isActive = false;
	
	behaviourState[instanceID] = Die;
	SetAnimation(instanceID, Dead);
}

void Enemy::DeathEnd(int instanceID)
{
	_transforms[instanceID]->isActive = false;
}

void Enemy::MoveTo(UINT instanceID)
{
	if (!_player)
		return;

	Vector3 direction = (_player->position - _transforms[instanceID]->position);
	direction.Normalize();

	_transforms[instanceID]->position += direction * Timer::Get().GetElapsedTime() * _moveSpeed;
}

void Enemy::UpdateAI(UINT instanceID)
{
	if (behaviourState[instanceID] == Die)
		return;

	CheckDistance(instanceID);
	switch (behaviourState[instanceID])
	{
	case None:
		SetIdle(instanceID);
		break;
	case Move:
		RotateTo(instanceID);
		MoveTo(instanceID);
		break;
	}
}

void Enemy::SetAnimation(UINT instanceID, EnemyAnimState value, float speed)
{
	if (animState[instanceID] != value)
	{
		animState[instanceID] = value;
		PlayClip(instanceID, animState[instanceID], speed);
	}
}
