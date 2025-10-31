#pragma once

#include "Scene.h"

class Terrain;
class Player;

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
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
};

