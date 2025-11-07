#pragma once

#include "Enemy.h"

class Factory
{
public:
	static Enemy* CreateEnemy(Enemy::EnemyType type, const vector<Vector3>& positions);
};

