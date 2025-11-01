#include "pch.h"
#include "InGameScene.h"
#include "Terrain.h"
#include "Player.h"
#include "Environment.h"
#include "Camera.h"
#include "Warrok.h"
#include "SphereCollider.h"

InGameScene::InGameScene()
{
	// Terrain
	_terrain = new Terrain();

	// Player
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->SetTerrain(_terrain);
	_player->_destPos = _player->position;
	Environment::Get().GetMainCamera()->SetTarget(_player);

	// Warrok
	_warrok = new Warrok("Warrok");
	Transform* transform = _warrok->AddTransform();
	transform->position = Vector3(50.0f, 0.0f, 50.0f);
	transform->scale = Vector3(0.1f);
	_warrok->UpdateTransforms();
	float radius = (_warrok->worldMinBox - _warrok->worldMaxBox).Length() * 0.5f;
	_warrok->mainCollider = new SphereCollider(radius * 0.25f);
	Vector3 offset = Vector3(0.0f, 10.0f, 0.0f);
	_warrok->mainCollider->SetTarget(transform, offset);
}

InGameScene::~InGameScene()
{
	delete _warrok;
	delete _player;
	delete _terrain;
}

void InGameScene::Update()
{
	_terrain->Update();
	_player->Update();
	_warrok->Update();

	PlayerAttackToWarrok();
}

void InGameScene::PreRender()
{
}

void InGameScene::Render()
{
	_terrain->Render();
	_player->Render();
	_warrok->Render();
}

void InGameScene::PostRender()
{
	_terrain->PostRender();
	_player->PostRender();
	_warrok->PostRender();
}

void InGameScene::PlayerAttackToWarrok()
{
	Collider* player = _player->GetMainCollider();
	if (!player->isActive)
		return;
	Collider* warrok = _warrok->mainCollider;
	if (!warrok->isActive)
		return;
	if (player->Collision(warrok))
	{
		_player->Attack();
		_warrok->Hit(0);
	}
}
