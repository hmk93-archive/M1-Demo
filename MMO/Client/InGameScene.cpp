#include "pch.h"
#include "InGameScene.h"
#include "Terrain.h"
#include "Player.h"
#include "Environment.h"
#include "Camera.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "Field.h"
#include "FieldWall.h"
#include "AStar.h"
#include "Node.h"
#include "Input.h"
#include "NavMesh.h"
#include "tinyxml2.h"
#include "Utility.h"
#include "ModelObject.h"
using namespace Utility;

InGameScene::InGameScene()
{
	InitScene();

	// For Field Wall Updates
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
	delete _navMesh;
	delete _astar;
	delete _terrain;
	for (ModelObject* model : _models)
		delete model;
}

void InGameScene::Update()
{
	for (ModelObject* model : _models)
		model->Update();
	_terrain->Update();
	_astar->Update();
	for (Field* field : _fields)
		field->Update();
	_player->Update();
	_warrok->Update();

	PlayerAttackToWarrok();
	UpdateEnemies();
}

void InGameScene::PreRender()
{
}

void InGameScene::Render()
{
	for (ModelObject* model : _models)
		model->Render();
	_terrain->Render();
	_astar->Render();
	_navMesh->Render();
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

void InGameScene::LoadMap()
{
	string file = "../Data/Main.xml";

	XmlDocument* document = new XmlDocument();
	document->LoadFile(file.c_str());

	XmlElement* root = document->FirstChildElement();

	XmlElement* modelNode = root->FirstChildElement();
	
	do
	{
		XmlElement* node = modelNode->FirstChildElement();

		string text = "";
		text = node->GetText();
		ModelObject* model = new ModelObject(text + "/" + text);

		Vector3 vec;
		node = node->NextSiblingElement();
		vec.x = node->FloatAttribute("x");
		vec.y = node->FloatAttribute("y");
		vec.z = node->FloatAttribute("z");
		model->scale = vec;

		node = node->NextSiblingElement();
		vec.x = node->FloatAttribute("x");
		vec.y = node->FloatAttribute("y");
		vec.z = node->FloatAttribute("z");
		model->rotation = vec;

		node = node->NextSiblingElement();
		vec.x = node->FloatAttribute("x");
		vec.y = node->FloatAttribute("y");
		vec.z = node->FloatAttribute("z");
		model->position = vec;

		modelNode = modelNode->NextSiblingElement();

		_models.push_back(model);

	} while (modelNode != nullptr);

	delete document;
}

void InGameScene::PlayerAttackToWarrok()
{
	Collider* player = _player->GetMainCollider();
	if (!player->isActive)
		return;
	Collider* warrok = _warrok->mainCollider[0];
	if (!warrok->isActive)
		return;
	if (_player->behaviourState != Player::PlayerBehaviourState::War)
		return;
	if (player->Collision(warrok))
	{
		_warrok->Damage(0, 0);
	}
}

void InGameScene::WarrokToMouse()
{
	Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Input::Get().GetMouse());
	Collider* col = _warrok->mainCollider[0];
	if (col->RayCollision(ray))
		_warrok->onMouse = true;
	else
		_warrok->onMouse = false;
}

void InGameScene::WarrokToPlayer()
{
	Collider* warrokCol = _warrok->mainCollider[0];
	Collider* playerMainCol = _player->GetMainCollider();
	Collider* playerEventCol = _player->GetEventCollider();
	if (warrokCol->Collision(playerEventCol))
	{
		if (Input::Get().Down('1') && _warrok->onMouse)
		{
			_player->LookAt(warrokCol->position - _player->position);
			_player->Attack(0);
		}
		if (Input::Get().Down('2') && _warrok->onMouse)
		{
			_player->LookAt(warrokCol->position - _player->position);
			_player->Attack(1);
		}

		if (warrokCol->Collision(playerMainCol))
			_player->PushBack(warrokCol);
	}
}

void InGameScene::SetCamera()
{
	Environment::Get().GetMainCamera()->position = { 0, 100.0f, -70.0f };
	Environment::Get().GetMainCamera()->rotation = { 0.6f, 0, 0 };
	Environment::Get().GetMainCamera()->mode = Camera::CamMode::Follow;
	Environment::Get().GetMainCamera()->SetTarget(_player);
}

void InGameScene::InitScene()
{
	// Map Object
	LoadMap();

	// Terrain
	_terrain = new Terrain();
	
	// AStar
	_astar = new AStar(100, 100);

	// NavMesh
	_navMesh = new NavMesh(_terrain->GetSize().x, _terrain->GetSize().y);
	// _navMesh->SetTerrain(_terrain);
	// _navMesh->Bake();

	// Fields
	for (UINT i = 0; i < 9; i++)
	{
		Field* field = new Field(_terrain, Field::Location(i));
		_fields.emplace_back(field);
	}
	_astar->SetNode(_terrain->GetSize());

	// Player
	CreatePlayer();

	// Enemies
	CreateEnemies();

	// Camera
	SetCamera();
}

void InGameScene::CreatePlayer()
{
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->SetTerrain(_terrain);
	_player->SetAStar(_astar);
	_player->SetNavMesh(_navMesh);
}

void InGameScene::CreateEnemies()
{
	if (!_player)
		return;

	// Warrok
	_warrok = new Enemy("Warrok");
	Transform* transform = _warrok->AddTransform();
	transform->position = Vector3(50.0f, 0.0f, 50.0f);
	transform->scale = Vector3(0.1f);
	_warrok->UpdateTransforms();
	_warrok->SetAnimation(0, Enemy::EnemyAnimState::Idle);
	_warrok->SetEndEvents(0, (UINT)Enemy::EnemyAnimState::Hit, bind(&Enemy::SetIdle, _warrok, 0));

	float radius = (_warrok->worldMinBox - _warrok->worldMaxBox).Length() * 0.5f;
	_warrok->mainCollider[0] = new SphereCollider(radius * 0.5f);
	
	Vector3 offset = Vector3(0.0f, 10.0f, 0.0f);
	_warrok->mainCollider[0]->SetTarget(transform, offset);
	_warrok->SetPlayer(_player);
}

void InGameScene::UpdateEnemies()
{
	_warrok->UpdateAI(0);
	_warrok->UpdateTransforms();

	WarrokToMouse();
	WarrokToPlayer();
}
