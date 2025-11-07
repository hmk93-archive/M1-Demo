#include "pch.h"
#include "Player.h"
#include "Terrain.h"
#include "Input.h"
#include "Timer.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "AStar.h"
#include "Environment.h"
#include "Camera.h"
#include "NavMesh.h"
#include "Utility.h"
#include "Cursor.h"
#include "ModelObject.h"
using namespace Utility;

Player::Player(string file)
	: ModelAnimator(file + "/" + file)
	, behaviourState(None)
{
	scale = Vector3(0.05f);

	SetShader(L"Default");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	ReadClip(file + "/Jump0", false);
	ReadClip(file + "/Attack_A0");
	ReadClip(file + "/Attack_B0", false);
	ReadClip(file + "/Hit0");

	SetEndEvent(Jump, bind(&Player::SetIdle, this));
	SetEndEvent(Attack_A, bind(&Player::AttackEnd, this));
	SetEndEvent(Attack_B, bind(&Player::AttackEnd, this));
	SetEndEvent(Impact, bind(&Player::SetIdle, this));

	SetFrameEvent(Attack_A, bind(&Player::ActiveWeaponCollider, this), { 32 });
	SetFrameEvent(Attack_B, bind(&Player::ActiveWeaponCollider, this), { 32 });

	SetAnimation(Idle);

	CreateCollider();
	CreateWeapons();

	_cursor = new Cursor();
}

Player::~Player()
{
	delete _cursor;
	delete _sword;
	delete _eventCollider;
	delete _mainCollider;
}

void Player::Update()
{
	_cursor->Update();

	Control();
	Move();
	Rotate();
	SetHeight();
	UpdateMatrix();

	ModelAnimator::Update();
}

void Player::Render()
{
	_cursor->Render();

	SetWorldBuffer();
	ModelAnimator::Render();

	_mainCollider->Render();
	_eventCollider->Render();

	_sword->Render();
}

void Player::PostRender()
{
}

Collider* Player::GetSwordCollider()
{
	return _sword->collider;
}

void Player::Attack(int type)
{
	if (behaviourState == War)
		return;
	
	switch (type)
	{
	case 0:
		SetAnimation(Attack_A);
		break;
	case 1:
		SetAnimation(Attack_B);
		break;
	}
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

void Player::LookAt(Vector3 direction)
{
	Utility::LookAt(rotation.y, direction);
}

void Player::Hit(int damage)
{
	if (behaviourState == Air || behaviourState == War)
		return;
	SetAnimation(Impact);
}

void Player::CreateCollider()
{
	// Main Collider
	Vector3 minBox, maxBox;
	SetBox(&minBox, &maxBox);
	_mainCollider = new CapsuleCollider(15.0f, 100.0f);

	// Event Collider
	float radius = (minBox - maxBox).Length() * 0.5f;
	_eventCollider = new SphereCollider(radius);
}

void Player::CreateWeapons()
{
	// rotation 값 주의
	// 이유에 대해서 아직 이해하지 못함
	// Blender Export 시 축이 변환되는건가??
	_sword = new ModelObject("Sword/Sword");
	_sword->rotation.x = XM_PIDIV2;
	_sword->rotation.z = -XM_PIDIV2;
	_sword->position = 0.05f * Vector3(-73.0f, 141.47f, 4.1882f);
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
	_rightHand = GetTransformByNode(i1) * _world;

	_mainCollider->SetParent(&_body);
	_eventCollider->SetParent(&_body);
	
	_sword->SetParent(&_rightHand);
}

void Player::Control()
{
	if (Environment::Get().GetMainCamera()->mode != Camera::CamMode::Follow)
		return;

	// Path Finding
	if (Input::Get().Down(VK_LBUTTON))
	{
		if (behaviourState != Air)
		{
			_velocity = {};
			SetPath();
		}
	}

	//// WSAD 
	//if (Input::Get().Press('W'))
	//{
	//	_path.clear();
	//	_velocity += Vector3::Backward;
	//	_velocity.Normalize();
	//}
	//if (Input::Get().Press('S'))
	//{
	//	_path.clear();
	//	_velocity += Vector3::Forward;
	//	_velocity.Normalize();
	//}
	//if (Input::Get().Press('A'))
	//{
	//	_path.clear();
	//	_velocity += Vector3::Left;
	//	_velocity.Normalize();
	//}
	//if (Input::Get().Press('D'))
	//{
	//	_path.clear();
	//	_velocity += Vector3::Right;
	//	_velocity.Normalize();
	//}

	if (Input::Get().Down(VK_SPACE) && behaviourState != War)
	{
		behaviourState = Air;
		SetAnimation(Jump);
	}
}

void Player::Move()
{
	if (behaviourState == War)
		return;

	SetVelocity();
	if (_velocity.Length() > 0.0f)
	{
		Vector3 zero = Vector3::Zero;
		_velocity = Vector3::Lerp(_velocity, zero, _deceleration * Timer::Get().GetElapsedTime());
	}

	position += _velocity * _moveSpeed * Timer::Get().GetElapsedTime();

	//@TODO: WSAD 사용 시 NavMesh 로 다음 위치 판정 필요

	// 공중에 있을 경우 다른 애니메이션 재생 하지 않음
	if (behaviourState == Air)
		return;

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
	if (behaviourState == War)
		return;
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
	if (behaviourState == War || behaviourState == Air)
		behaviourState = None;

	_sword->collider->isActive = false;

	SetAnimation(Idle);
}

void Player::AttackEnd()
{
	SetIdle();
}

void Player::ActiveWeaponCollider()
{
	_sword->collider->isActive = true;
	behaviourState = War;
}

void Player::SetPath()
{
	if (!_terrain || !_astar)
		return;

	_path.clear();
	_terrain->ComputePicking(_destPos);
	_astar->SetPath(_path, position, _destPos);


	wstring log = to_wstring(_path.size()) + L"\n";
	OutputDebugString(log.c_str());
}

void Player::SetVelocity()
{
	// Path Finding
	if (!_path.empty())
	{
		Vector3 dest = _path.back();
		Vector3 direction = dest - position;
		if (direction.Length() < 0.1f)
			_path.pop_back();

		_velocity = direction;
		_velocity.Normalize();
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
	if (animState != value)
	{
		animState = value;
		PlayClip(animState, speed);
	}
}

void Player::SetTerrain(Terrain* terrain)
{
	_terrain = terrain;
	if (_cursor)
		_cursor->SetTerrain(_terrain);
}
