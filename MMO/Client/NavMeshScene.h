#pragma once

#include "Scene.h"

class Terrain;
class NavMesh;
class Player;

class NavMeshScene : public Scene
{
public:
	NavMeshScene(bool showConsole = false);
	~NavMeshScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void BakeTest();

	void CreatePlayer();

private:
	Terrain* _terrain = nullptr;
	NavMesh* _navMesh = nullptr;
	Player* _player = nullptr;
};

