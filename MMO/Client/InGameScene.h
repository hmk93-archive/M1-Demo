#pragma once

#include "Scene.h"

class Terrain;
class Player;
class Warrok;
class Field;

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
	void PlayerAttackToWarrok();

private:
	Terrain* _terrain = nullptr;
	Player* _player = nullptr;
	Warrok* _warrok = nullptr;

	vector<Field*> _fields = {};
};

