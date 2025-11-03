#include "pch.h"
#include "NavMeshScene.h"
#include "Terrain.h"
#include "NavMesh.h"

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
}

NavMeshScene::~NavMeshScene()
{
	delete _navMesh;
	delete _terrain;
}

void NavMeshScene::Update()
{
	// _terrain->Update();
}

void NavMeshScene::PreRender()
{
}

void NavMeshScene::Render()
{
	_terrain->Render();
	_navMesh->Render();
}

void NavMeshScene::PostRender()
{
	// _terrain->PostRender();
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
