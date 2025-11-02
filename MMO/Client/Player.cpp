#include "pch.h"
#include "Player.h"
#include "Terrain.h"
#include "Control.h"
#include "Timer.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "AStar.h"

Player::Player(string file)
	: ModelAnimator(file + "/" + file)
	, behaviourState(None)
{
	scale = Vector3(0.05f);

	SetShader(L"Default");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	ReadClip(file + "/Attack_A0", false);

	SetEndEvent(Attack_A, bind(&Player::SetIdle, this));
	
	SetAnimation(Idle);

	CreateCollider();
}

Player::~Player()
{
	delete _eventCollider;
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
	_eventCollider->Render();
}

void Player::PostRender()
{

}

void Player::Attack()
{
	if (behaviourState == War)
		return;
	behaviourState = War;
	SetAnimation(Attack_A);
}

void Player::PushBack(Collider* other)
{
	_path.clear();
	_velocity = Vector3::Zero;
	Vector3 p1 = other->GlobalPos();
	Vector3 p2 = _eventCollider->GlobalPos();
	Vector3 dir = (p1 - p2);
	float r1 = ((SphereCollider*)other)->Radius();
	float r2 = ((SphereCollider*)_eventCollider)->Radius();
	float d = (p1 - p2).Length();
	float overlap = (r1 + r2) - d;
	position -= (dir * overlap) * Timer::Get().GetElapsedTime();
}

void Player::CreateCollider()
{
	Vector3 minBox, maxBox;
	SetBox(&minBox, &maxBox);
	_mainCollider = new CapsuleCollider(15.0f, 100.0f);

	float radius = (minBox - maxBox).Length() * 0.5f;
	_eventCollider = new SphereCollider(radius);
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
	_eventCollider->SetParent(&_body);
}

void Player::Control()
{
	if (Control::Get().Down(VK_LBUTTON))
	{
		SetPath();
	}
}

void Player::Move()
{
	if (behaviourState == Jump || behaviourState == War)
		return;

	SetVelocity();
	if (_velocity.Length() > 0.0f)
	{
		Vector3 zero = Vector3::Zero;
		_velocity = Vector3::Lerp(_velocity, zero, _deceleration * Timer::Get().GetElapsedTime());
	}

	position += _velocity * _moveSpeed * Timer::Get().GetElapsedTime();

	if (_velocity.Length() > 0.5f)
	{
		SetAnimation(Run);
	}
	else
	{
		if (animState == Run)
		{
			SetAnimation(Idle);
		}
	}
}

void Player::Rotate()
{
	if (_velocity.Length() < 0.1f)
		return;
	Vector3 start = Forward() * -1.0f;
	Vector3 end = _velocity;
	end.Normalize();
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

void Player::SetIdle()
{
	SetAnimation(Idle);
	if (behaviourState == War || behaviourState == Jump)
		behaviourState = None;
}

void Player::SetPath()
{
	if (!_terrain || !_astar)
		return;

	_path.clear();
	_terrain->ComputePicking(_destPos);
	_astar->SetPath(_path, position, _destPos);
}

void Player::SetVelocity()
{
	if (_path.empty())
		return;

	Vector3 dest = _path.back();
	Vector3 direction = dest - position;
	if (direction.Length() < 0.1f)
		_path.pop_back();

	_velocity = direction;
	_velocity.Normalize();
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
	if (animState != value)
	{
		animState = value;
		PlayClip(animState, speed);
	}
}
