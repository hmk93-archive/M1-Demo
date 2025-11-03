#pragma once

#include "Scene.h"

class Terrain;
class Player;
class Warrok;
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
	void Create();

	void CreatePlayer();
	void CreateEnemies();

	void LoadMap();
	void PlayerAttackToWarrok();
	void WarrokToMouse();
	void WarrokToPlayer();

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
	Warrok* _warrok = nullptr;

	vector<Field*> _fields = {};

	AStar* _astar = nullptr;

	NavMesh* _navMesh = nullptr;

	vector<ModelObject*> _models = {};
};

