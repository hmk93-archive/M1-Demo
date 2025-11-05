#pragma once

#include "Transform.h"

class Circle;
class Terrain;

class Cursor : public Transform
{
public:
	Cursor();
	~Cursor();

	void Update();
	void Render();

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }

private:
	void PlayAnimation();
	void Animation();
	void ResetAnimation();

	void RenderCircle();

private:
	Circle* _circle = nullptr;
	Terrain* _terrain = nullptr;
	bool _isPlay = false;
};

