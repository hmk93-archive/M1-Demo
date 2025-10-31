#pragma once

class Game
{
public:
	Game();
	~Game();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	void RenderFPS();

public:
	static bool s_exit;

	// TMP
	class Cube* _cube;
	class Sphere* _sphere;
	class ModelObject* _modelObj;
	class Player* _player;
	class Terrain* _terrain;
};

