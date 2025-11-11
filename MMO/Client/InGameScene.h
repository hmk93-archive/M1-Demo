#pragma once

#include "Scene.h"

class Terrain;
class Player;
class Enemy;
class Field;
class AStar;
class NavMesh;
class ModelObject;
class Shadow;

class InGameScene : public Scene
{
public:
	InGameScene();
	~InGameScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	Enemy* GetEnemies() { return _enemies; }

private:
	void InitScene();

	void CreatePlayer();
	void CreateEnemies();

	void UpdateEnemies();

	void LoadMap();
	void PlayerAttackToEnemy();
	void EnemyToMouse();
	void EnemyToPlayer();

	void SetCamera();

	void SetAStarObstacles();

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
	Enemy* _enemies = nullptr;

	UINT _drawWarrokCount = 1;

	vector<Field*> _fields = {};

	AStar* _astar = nullptr;

	NavMesh* _navMesh = nullptr;

	vector<ModelObject*> _models = {};

	Shadow* _shadow = nullptr;
};

