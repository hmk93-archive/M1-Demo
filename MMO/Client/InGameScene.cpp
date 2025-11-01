#include "pch.h"
#include "InGameScene.h"
#include "Terrain.h"
#include "Player.h"
#include "Environment.h"
#include "Camera.h"
#include "Warrok.h"
#include "SphereCollider.h"
#include "Field.h"
#include "FieldWall.h"
#include "AStar.h"
#include "Node.h"

InGameScene::InGameScene()
{
	// Terrain
	_terrain = new Terrain();

	// AStar
	_astar = new AStar(100, 100);

	// Fields
	for (UINT i = 0; i < 9; i++)
	{
		Field* field = new Field(_terrain, Field::Location(i));
		_fields.emplace_back(field);
	}
	_astar->SetNode(_terrain->GetSize());

	// Player
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->SetTerrain(_terrain);
	_player->SetAStar(_astar);
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

	Update();

	// Set AStar Obstacle
	UINT width = _astar->width;
	UINT height = _astar->height;
	UINT patchWidth = ((float)_astar->width / 3.0f);
	UINT patchHeight = ((float)_astar->height / 3.0f);
	UINT cnt = 0;
	for (UINT i = 0; i < _fields.size(); ++i)
		for (FieldWall* wall : _fields[i]->walls)
		{
			Collider* wallCol = wall->GetCollider();
			UINT startX = (i % 3) * patchWidth;
			UINT startZ = (i / 3) * patchHeight;
			for (UINT z = startZ; z < startZ + patchHeight; ++z)
				for (UINT x = startX; x < startX + patchWidth; ++x)
				{
					UINT idx = z * width + x;
					idx = clamp(idx, (UINT)0, (UINT)_astar->nodes.size() - 1);

					Node* node = _astar->nodes[idx];
					if (node->state == Node::State::Obstacle)
						continue;

					Collider* nodeCol = node->GetCollider();
					if (nodeCol->Collision(wallCol))
					{
						_astar->MakeObsAt(idx);
						cnt++;
					}
				}
		}
}

InGameScene::~InGameScene()
{
	delete _warrok;
	delete _player;
	for (Field* field : _fields)
		delete field;
	delete _astar;
	delete _terrain;
}

void InGameScene::Update()
{
	_terrain->Update();
	_astar->Update();
	for (Field* field : _fields)
		field->Update();
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
	_astar->Render();
	for (Field* field : _fields)
		field->Render();
	_player->Render();
	_warrok->Render();
}

void InGameScene::PostRender()
{
	_terrain->PostRender();
	_astar->PostRender();
	for (Field* field : _fields)
		field->PostRender();
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
