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
	_navMesh = new NavMesh(_terrain->GetSize().x, _terrain->GetSize().y, false);

	auto& vertices = _terrain->GetVertices();
	auto& indices = _terrain->GetIndices();
	vector<Vector3> newVertices(vertices.size());
	int i = 0;
	for (auto& v : vertices)
		newVertices[i++] = vertices[i].position;

	_navMesh->AppedMesh(newVertices, indices);

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
	
}

void NavMeshScene::CreatePlayer()
{
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->SetTerrain(_terrain);
	_player->SetNavMesh(_navMesh);
	Environment::Get().GetMainCamera()->SetTarget(_player);
}
