#include "pch.h"
#include "NavMeshScene.h"
#include "TerrainEditor.h"
#include "Terrain.h"
#include "NavMesh.h"
#include "Player.h"
#include "Environment.h"
#include "Camera.h"

NavMeshScene::NavMeshScene(bool showConsole)
{
	if (showConsole)
		CreateConsole();

	// Terrain
	_terrain = new Terrain();

	// NavMesh
	_navMesh = new NavMesh(_terrain->GetSize().x, _terrain->GetSize().y);
	_navMesh->SetTerrain(_terrain);
	_navMesh->Bake();
	// BakeTest();

	// Player
	CreatePlayer();
}

NavMeshScene::~NavMeshScene()
{
	delete _player;
	delete _navMesh;
	delete _terrain;
}

void NavMeshScene::Update()
{
	_terrain->Update();
	_player->Update();
}

void NavMeshScene::PreRender()
{
}

void NavMeshScene::Render()
{
	_terrain->Render();
	_navMesh->Render();
	_player->Render();
}

void NavMeshScene::PostRender()
{
	_terrain->PostRender();
	_player->PostRender();

	ImGui::Begin("[NavMesh]");
	ImGui::Checkbox("Terrain Draw", &_terrain->isActive);
	ImGui::Checkbox("navMesh Draw", &_navMesh->isActive);
	ImGui::Checkbox("Player Draw", &_player->isActive);
	ImGui::End();
}

void NavMeshScene::BakeTest()
{
	vector<vector<float>>& heightMap = _navMesh->_heightMap;
	vector<vector<bool>>& walkableMap = _navMesh->_walkableMap;
	for (auto ele : heightMap)
	{
		for (auto height : ele)
		{
			cout << height << " ";
		}
		cout << endl;
	}

	for (UINT z = 0; z < 300; z++)
	{
		for (UINT x = 0; x < 300; x++)
		{
			if (_navMesh->PointInTriangle(Vector3(x, 0.0f, z)))
			{
				cout << true << " ";
			}
			else
			{
				cout << false << " ";
			}
		}
		cout << endl;
	}
}

void NavMeshScene::CreatePlayer()
{
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->SetTerrain(_terrain);
	_player->SetNavMesh(_navMesh);
	Environment::Get().GetMainCamera()->SetTarget(_player);
}
