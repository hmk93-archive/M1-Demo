#include "pch.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "Player.h"
#include "Timer.h"
#include "Utility.h"
#include "HUDHPBar.h"
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
	for (UINT i = 0; i < _drawCount; i++)
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

	for (UINT i = 0; i < _drawCount; i++)
		mainCollider[i]->Render();
}

void Enemy::PostRender()
{
	for (UINT i = 0; i < _drawCount; i++)
		hpBar[i]->Render();	
}

void Enemy::Damage(UINT instanceID, UINT damage)
{
	if (behaviourState == Die)
		return;

	mainCollider[instanceID]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	behaviourState = War;

	SetAnimation(instanceID, EnemyAnimState::Hit);

	SetHP(instanceID, damage);
}

void Enemy::CheckDistance(UINT instanceID)
{
	if (!_player)
		return;

	if (behaviourState == War)
		return;

	const float distance = (_player->position - _transforms[instanceID]->position).Length();
	if (distance > _attackRange)
	{
		SetAnimation(instanceID, Idle);
		state = Idle;
	}
	else
	{
		SetAnimation(instanceID, Run);
		state = Run;
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
	behaviourState = None;
	SetAnimation(instanceID, Idle);
}

void Enemy::SetHP(UINT instanceID, int damage)
{
	_curHP -= damage;
	if (_curHP <= 0)
		Death(instanceID);

	float hpRatio = (float)_curHP / (float)_maxHP;
	hpBar[instanceID]->isHit = true;
	hpBar[instanceID]->SetScale(HUDHPBar::Face, hpRatio);
	hpBar[instanceID]->SetCurrentBodyScale();
}

HUDHPBar* Enemy::AddHPBar()
{
	assert(_drawCount - 1 >= 0);
	HUDHPBar* bar = new HUDHPBar();
	hpBar[_drawCount - 1] = bar;
	return bar;
}

void Enemy::Death(UINT instanceID)
{
	_curHP = 0;

	mainCollider[instanceID]->isActive = false;
	hpBar[instanceID]->isActive = false;

	behaviourState = Die;
	
	SetAnimation(instanceID, Dead);
}

void Enemy::DeathEnd(int instanceID)
{
	_transforms[instanceID]->isActive = false;

	// @TODO: Respawn
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
	if (behaviourState == Die)
		return;

	CheckDistance(instanceID);
	switch (state)
	{
	case Idle:
		SetIdle(instanceID);
		break;
	case Run:
		RotateTo(instanceID);
		MoveTo(instanceID);
		break;
	}
}

void Enemy::SetAnimation(UINT instanceID, EnemyAnimState value, float speed)
{
	if (state != value)
	{
		state = value;
		PlayClip(instanceID, state, speed);
	}
}
