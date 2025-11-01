#include "pch.h"
#include "Field.h"
#include "Terrain.h"
#include "FieldWall.h"

vector<string> Field::locationNames
{
	"LB", "CB", "RB",
	"LM", "CM", "RM",
	"LT", "CT", "RT"
};

Field::Field(Terrain* terrain, Location location)
	: terrain(terrain)
	, location(location)
{
	//@TODO: terrain 을 나누어서 관리
	fieldSize.x = terrain->GetSize().x / 3.0f;
	fieldSize.y = terrain->GetSize().y / 3.0f;

	fieldPos.x = (int)location % 3 * fieldSize.x;
	fieldPos.z = (int)location / 3 * fieldSize.y;

	CreateWalls();
}

Field::~Field()
{
	for (FieldWall* wall : walls)
		delete wall;
	walls.clear();
}

void Field::Update()
{
	for (FieldWall* wall : walls)
		wall->Update();
}

void Field::Render()
{
	for (FieldWall* wall : walls)
		wall->Render();
}

void Field::PostRender()
{

}

void Field::CreateWalls()
{
	Vector2 wallSize = fieldSize;
	wallSize.y /= 10.0f;
	wallSize.x *= 0.4f;
	wallSize.y *= 0.4f;

	vector<Vector3> wallPos =
	{
		Vector3(0, 0, 0),
		Vector3(fieldSize.x, 0, 0),
		Vector3(fieldSize.x, 0, fieldSize.y),
		Vector3(0, 0, fieldSize.y)
	};

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			FieldWall* wall = new FieldWall(wallSize, { 10, 1 });
			wall->position = wallPos[i] + fieldPos;
			wall->rotation.y = -XM_PIDIV2 * i - XM_PIDIV2 * j;
			walls.emplace_back(wall);
		}
	}
}

void Field::CreateCube()
{
}
