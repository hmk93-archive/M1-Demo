#pragma once

#include "Scene.h"

class Terrain;
class NavMesh;

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

private:
	Terrain* _terrain = nullptr;
	NavMesh* _navMesh = nullptr;
};

