#pragma once

class Terrain;
class FieldWall;
class Cube;

class Field
{
public:
	enum Location
	{
		LB, CB, RB,
		LM, CM, RM,
		LT, CT, RT,
	} location;

	static vector<string> locNames;
	string tag;

	enum MapState
	{
		NORMAL, 
		ALERT, 
		DANGER
	} state;

	static vector<string> stateNames;

public:
	Field(Terrain* terrain, Location location);
	~Field();

	void Update();
	void Render();
	void PostRender();
	void SetState();
	void RenderAlertTime();

private:
	void SetTag();
	void SetFieldData();
	void SetRandomEmissive();
	void CreateWalls();
	void CreateCube();
	void AlertToDanger();

public:
	vector<FieldWall*> walls;
	Cube* cube;

public:
	Terrain* terrain;
	Vector3 fieldPos;
	Vector2 fieldSize;

private:
	float _alertTime = 0.0f;
};