#include "pch.h"
#include "Player.h"
#include "Terrain.h"
#include "Control.h"
#include "Timer.h"
#include "CapsuleCollider.h"

Player::Player(string file)
	: ModelAnimator(file + "/" + file)
{
	SetShader(L"Default");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	
	SetAnimation(Idle);

	CreateCollider();
}

Player::~Player()
{
	delete _mainCollider;
}

void Player::Update()
{
	Control();
	Move();
	Rotate();
	SetHeight();
	UpdateMatrix();

	ModelAnimator::Update();
}

void Player::Render()
{
	SetWorldBuffer();
	ModelAnimator::Render();

	_mainCollider->Render();
}

void Player::PostRender()
{

}

void Player::CreateCollider()
{
	_mainCollider = new CapsuleCollider(15.0f, 100.0f);
}

void Player::UpdateMatrix()
{
	static bool set = false;
	static int i1, i2, i3, i4;
	if (!set)
	{
		set = true;
		i1 = GetNodeByName("mixamorig:RightHand");
		i2 = GetNodeByName("mixamorig:Spine1");
		i3 = GetNodeByName("mixamorig:Hips");
		i4 = GetNodeByName("mixamorig:Neck");
	}

	_body = GetTransformByNode(i2) * _world;

	_mainCollider->SetParent(&_body);
}

void Player::Control()
{
	if (Control::Get().Press(VK_LBUTTON))
	{
		_terrain->ComputePicking(_destPos);
	}
}

void Player::Move()
{
	SetVelocity();

	position += _velocity * _moveSpeed * Timer::Get().GetElapsedTime();

	if (_velocity.Length() > 0.5f)
	{
		SetAnimation(Run);
	}
	else
	{
		if (state == Run)
		{
			SetAnimation(Idle);
		}
	}
}

void Player::Rotate()
{
	if (_velocity.Length() < 0.1f)
		return;
	_velocity.Normalize();
	Vector3 start = Forward() * -1.0f;
	Vector3 end = _velocity;
	float cosValue = start.Dot(end);
	float theta = acos(cosValue);
	if (theta < 0.1f)
		return;
	Vector3 cross = start.Cross(end);
	if (cross.y > 0.0f)
		rotation.y += _rotateSpeed * Timer::Get().GetElapsedTime();
	else
		rotation.y -= _rotateSpeed * Timer::Get().GetElapsedTime();
}

void Player::SetVelocity()
{
	Vector3 direction = _destPos - position;
	_velocity = direction;
	//@TODO
	_velocity.y = 0.0f; 
	_velocity.Normalize();

	// ¸¶Âû·Â Àû¿ë
	if (_velocity.Length() > 0.0f)
	{
		Vector3 zero = Vector3(0.0f);
		_velocity = Vector3::Lerp(_velocity, zero, _deceleration * Timer::Get().GetElapsedTime());
	}
}

void Player::SetHeight()
{
	if (!_terrain)
		return;

	float height = _terrain->GetHeight(position);

	position.y = height;
}

void Player::SetAnimation(PlayerAnimState value, float speed)
{
	if (state != value)
	{
		state = value;
		PlayClip(state, speed);
	}
}
