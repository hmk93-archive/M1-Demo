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
#include "Shadow.h"
#include "Factory.h"
#include "UIManager.h"
#include "Model.h"
#include "Material.h"
#include "ModelMesh.h"
#include "BoxCollider.h"
using namespace Utility;

InGameScene::InGameScene()
{
	InitScene();
	Update(); // For Field Wall Updates
	SetAStarObstacles();

	_shadow = new Shadow();
}

InGameScene::~InGameScene()
{
	delete _shadow;
	delete _enemies;
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
	_enemies->Update();

	PlayerAttackToEnemy();
	UpdateEnemies();
}

void InGameScene::PreRender()
{
	_shadow->PreRender();

	for (ModelObject* model : _models)
	{
		model->GetModel()->SetShader(L"Depth");
		model->Render();
	}

	_terrain->GetMaterial()->SetShader(L"Depth");
	_terrain->Render();

	for (Field* field : _fields)
		field->Render();

	_player->SetShader(L"Depth");
	_player->Render();

	_enemies->SetShader(L"Depth");
	_enemies->Render();
}

void InGameScene::Render()
{
	_shadow->Render();

	for (ModelObject* model : _models)
	{
		model->GetModel()->SetShader(L"Default");
		model->Render();
	}

	_terrain->GetMaterial()->SetShader(L"Terrain");
	_terrain->Render();

	for (Field* field : _fields)
		field->Render();

	_player->SetShader(L"Default");
	_player->Render();

	_enemies->SetShader(L"Instancing");
	_enemies->Render();

	UIManager::Get().Render();
}

void InGameScene::PostRender()
{
	_shadow->PostRender();

	_terrain->PostRender();
	_astar->PostRender();
	for (Field* field : _fields)
		field->PostRender();
	_player->PostRender();
	_enemies->PostRender();
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

void InGameScene::PlayerAttackToEnemy()
{
	for (UINT i = 0; i < _drawWarrokCount; i++)
	{
		Collider* weaponCol = _player->GetSwordCollider();
		if (!weaponCol->isActive)
			break;

		Collider* warrokCol = _enemies->mainCollider[i];
		if (!warrokCol->isActive)
			continue;

		if (_player->behaviourState != Player::PlayerBehaviourState::War)
			break;

		if (weaponCol->Collision(warrokCol))
		{
			weaponCol->isActive = false;
			_enemies->Damage(i, _player->GetAttackDamage());
		}
	}
}

void InGameScene::EnemyToMouse()
{
	for (UINT i = 0; i < _drawWarrokCount; i++)
	{
		Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Input::Get().GetMouse());
		Collider* col = _enemies->mainCollider[i];
		if (col->RayCollision(ray))
			_enemies->onMouse[i] = true;
		else
			_enemies->onMouse[i] = false;
	}
}

void InGameScene::EnemyToPlayer()
{
	for (UINT i = 0; i < _drawWarrokCount; i++)
	{
		Collider* warrokCol = _enemies->mainCollider[i];
		if (!warrokCol->isActive)
			continue;

		Collider* playerMainCol = _player->GetMainCollider();
		Collider* playerEventCol = _player->GetEventCollider();
		if (warrokCol->Collision(playerEventCol))
		{
			//_player->LookAt(warrokCol->position - _player->position);
			//_player->LookAt(warrokCol->position - _player->position);

			if (warrokCol->Collision(playerMainCol))
				_player->PushBack(warrokCol);
		}
	}
}

void InGameScene::SetCamera()
{
	Environment::Get().GetMainCamera()->position = { 0, 0.0f, 0.0f };
	Environment::Get().GetMainCamera()->rotation = { 0.6f, 0, 0 };
	Environment::Get().GetMainCamera()->mode = Camera::CamMode::Follow;
	Environment::Get().GetMainCamera()->SetTarget(_player);
}

void InGameScene::SetAStarObstacles()
{
	UINT width = _astar->width;
	UINT height = _astar->height;
	UINT patchWidth = ((float)_astar->width / 3.0f);
	UINT patchHeight = ((float)_astar->height / 3.0f);
	UINT cnt = 0;
	// Field Wall
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
	// ModelObject
	if (_models.empty())
		return;
	_astar->SetObstacle(_models);
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
	for (ModelObject* model : _models)
	{
		// @TODO:??
		BoxCollider* boxCollider = (BoxCollider*)model->collider;
		model->UpdateWorld();
		boxCollider->UpdateWorld();

		Vector3 minBox = boxCollider->MinBox();
		Vector3 maxBox = boxCollider->MaxBox();

		vector<Vector3> modelVertices;
		modelVertices.emplace_back(minBox.x, minBox.y, minBox.z);
		modelVertices.emplace_back(minBox.x, maxBox.y, minBox.z);
		modelVertices.emplace_back(maxBox.x, maxBox.y, minBox.z);
		modelVertices.emplace_back(maxBox.x, minBox.y, minBox.z);

		modelVertices.emplace_back(minBox.x, minBox.y, maxBox.z);
		modelVertices.emplace_back(minBox.x, maxBox.y, maxBox.z);
		modelVertices.emplace_back(maxBox.x, maxBox.y, maxBox.z);
		modelVertices.emplace_back(maxBox.x, minBox.y, maxBox.z);

		vector<UINT> modelIndices =
		{
			0, 1, 1, 2, 2, 3, 3, 0,
			4, 5, 5, 6, 6, 7, 7, 4,
			0, 4, 1, 5, 2, 6, 3, 7
		};
	
		_navMesh->AppendMesh(modelVertices, modelIndices);
	}

	auto& vertices = _terrain->GetVertices();
	auto& indices = _terrain->GetIndices();
	vector<Vector3> terrainVertices(vertices.size());
	int i = 0;
	for (auto& v : vertices)
		terrainVertices[i++] = vertices[i].position;

	_navMesh->AppendMesh(terrainVertices, indices);
	
	_navMesh->Bake();

	//// Fields
	//for (UINT i = 0; i < 9; i++)
	//{
	//	Field* field = new Field(_terrain, Field::Location(i));
	//	_fields.emplace_back(field);
	//}

	// Set AStar Node
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
	vector<Vector3> positions;
	for (UINT i = 0; i < _drawWarrokCount; i++)
	{
		Vector3 randomPos = Vector3(Random(0.0f, 300.0f), 0.0f, Random(0.0f, 300.0f));
		positions.push_back(randomPos);
	}
	_enemies = Factory::CreateEnemy(Enemy::EnemyType::Warrok, positions);
	_enemies->SetPlayer(_player);
}

void InGameScene::UpdateEnemies()
{
	for (UINT i = 0; i < _drawWarrokCount; i++)
		_enemies->UpdateAI(i);

	_enemies->UpdateTransforms();

	EnemyToMouse();
	EnemyToPlayer();
}
