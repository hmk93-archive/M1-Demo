#pragma once

#include "Scene.h"

class Terrain;
class Player;
class Enemy;
class Field;
class AStar;
class NavMesh;
class ModelObject;

class InGameScene : public Scene
{
public:
	InGameScene();
	~InGameScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void InitScene();

	void CreatePlayer();
	void CreateEnemies();

	void UpdateEnemies();

	void LoadMap();
	void PlayerAttackToWarrok();
	void WarrokToMouse();
	void WarrokToPlayer();

	void SetCamera();

	void SetAStarObstacles();

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
	Enemy* _warrok = nullptr;

	vector<Field*> _fields = {};

	AStar* _astar = nullptr;

	NavMesh* _navMesh = nullptr;

	vector<ModelObject*> _models = {};
};

