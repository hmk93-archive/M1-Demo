#include "pch.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "Player.h"
#include "Timer.h"
#include "Utility.h"
using namespace Utility;

Enemy::Enemy(string file)
	: ModelAnimators(file + "/" + file)
{
	SetShader(L"Instancing");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	ReadClip(file + "/Punch0");
	ReadClip(file + "/Hit0");
}

Enemy::~Enemy()
{
	for(Collider* collider : mainCollider)
		delete collider;
}

void Enemy::Update()
{
	for (UINT i = 0; i < _drawCount; i++)
		mainCollider[i]->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));

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

}

void Enemy::Damage(UINT instanceID, UINT damage)
{
	mainCollider[instanceID]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	behaviourState = War;

	SetAnimation(instanceID, EnemyAnimState::Hit);
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
