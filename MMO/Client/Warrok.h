#pragma once

#include "ModelAnimators.h"

class Terrain;
class Collider;

class Warrok : public ModelAnimators
{
	enum WarrokAnimState
	{
		Idle,
	} state;

public:
	Warrok(string file);
	~Warrok();

	void Update();

	void Render();
	void PostRender();

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }

	void Hit(UINT instanceID, UINT damage);

public:
	bool onMouse = false;

private:
	void SetAnimation(WarrokAnimState value, float speed = 1.0f);

public:
	Collider* mainCollider[MAX_INSTANCE] = {};

private:
	Terrain* _terrain = nullptr;
};

