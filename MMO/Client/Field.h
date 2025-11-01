#pragma once

class Terrain;
class FieldWall;

class Field
{
public:
	enum Location
	{
		LB, CB, RB,
		LM, CM, RM,
		LT, CT, RT
	} location;

	static vector<string> locationNames;

public:
	Field(Terrain* terrain, Location location);
	~Field();

	void Update();
	void Render();
	void PostRender();

private:
	void CreateWalls();
	void CreateCube();

public:
	Terrain* terrain = nullptr;
	Vector3 fieldPos = Vector3::Zero;
	Vector2 fieldSize = Vector2::Zero;

	vector<FieldWall*> walls = {};
};

