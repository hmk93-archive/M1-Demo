#include "pch.h"
#include "Field.h"
#include "Terrain.h"
#include "FieldWall.h"
#include "Cube.h"
#include "Timer.h"
#include "RasterizerState.h"

vector<string> Field::locNames
{
	"LB", "CB", "RB",
	"LM", "CM", "RM",
	"LT", "CT", "RT"
};

vector<string> Field::stateNames
{ "NORMAL", "ALERT", "DANGER" };

Field::Field(Terrain* terrain, Location location)
	:location(location), state(MapState::NORMAL),
	terrain(terrain)
{
	SetTag();
	SetFieldData();
	SetRandomEmissive();
	CreateWalls();
	CreateCube();
}

Field::~Field()
{
	delete cube;
	for (FieldWall* wall : walls)
		delete wall;
	walls.clear();
}

void Field::Update()
{
	for (FieldWall* wall : walls)
		wall->Update();
	//
	// cube->Update();
	//
	AlertToDanger();
}

void Field::Render()
{
	for (FieldWall* wall : walls)
		wall->Render();
	//
	// cube->Render();
}

void Field::PostRender()
{
	{// field State
		ImGui::Text(("State : " + stateNames[state]).c_str());
		if (ImGui::RadioButton("NORMAL", (int*)&state, 0))
			SetState();
		ImGui::SameLine();
		if (ImGui::RadioButton("ALERT", (int*)&state, 1))
			SetState();
		ImGui::SameLine();
		if( ImGui::RadioButton("DANGER", (int*)&state, 2))
			SetState();
	}
	// field pos
	ImGui::Text("Pos : [%.3f, %.3f]", fieldPos.x, fieldPos.y);
	cube->PostRender();
}

void Field::SetState()
{
	//switch (state)
	//{
	//case Field::NORMAL:
	//	cube->isActive = false;
	//	cube->SetColor(COLOR_WHITE);
	//	cube->IsRotate(false);
	//	break;
	//case Field::ALERT:
	//	cube->isActive = true;
	//	//cube->SetAnimColor(COLOR_RED, COLOR_WHITE , 0.5f);
	//	cube->SetAnimColor(COLOR_WHITE, COLOR_YELLOW, 0.5f);
	//	cube->IsAnimColor(true);
	//	cube->IsRotate(true);
	//	break;
	//case Field::DANGER:
	//	cube->isActive = true;
	//	cube->SetColor(COLOR_RED);
	//	cube->IsAnimColor(false);
	//	cube->IsRotate(false);
	//	cube->rotation.y = 0;
	//	break;
	//default:
	//	break;
	//}
}

void Field::RenderAlertTime()
{
	//if (state != MapState::ALERT) return;
	////
	//Vector3 pos = WorldToScreen(fieldPos);
	//pos.y = WIN_HEIGHT - pos.y;
	//FONT->RenderText(ToWString((int)alertTime), pos);
}

void Field::SetTag()
{
	tag = "Field [" + locNames[location] + "]";
}

void Field::SetFieldData()
{
	fieldSize.x = (terrain->GetSize().x) / 3.0f;
	fieldSize.y = (terrain->GetSize().y) / 3.0f;
	//
	fieldPos.x = (int)location % 3 * fieldSize.x;
	fieldPos.z = (int)location / 3 * fieldSize.y;
}

void Field::SetRandomEmissive()
{
	//Float4 color = {
	//	Random(0.0f, 1.0f),
	//	Random(0.0f, 1.0f),
	//	Random(0.0f, 1.0f),
	//	Random(0.5f, 1.0f)
	//};
	//matBuffer->data.emissive = color;
}

void Field::CreateWalls()
{
	Vector2 wallSize = fieldSize;
	wallSize.y /= 10.0f;
	//
	wallSize.x *= 0.4f;
	wallSize.y *= 0.4f;
	//
	vector<Vector3> wallPos =
	{
		Vector3(0, 0, 0),
		Vector3(fieldSize.x, 0, 0),
		Vector3(fieldSize.x, 0, fieldSize.y),
		Vector3(0, 0, fieldSize.y)
	};
	//
	for (int j = 0; j < 2; ++j)
		for (int i = 0; i < 4; ++i)
		{
			FieldWall* wall = new FieldWall(wallSize, { 10, 1 });
			//
			wall->position = wallPos[i] + fieldPos;
			//
			wall->rotation.y = -XM_PIDIV2 * i - XM_PIDIV2 * j;
			walls.emplace_back(wall);
		}
}

void Field::CreateCube()
{
	cube = new Cube;
	//
	cube->scale.x = fieldSize.x * 0.5f;
	cube->scale.z = fieldSize.x * 0.5f;
	cube->scale.y = fieldSize.x * 0.1f;
	//
	cube->position.x = fieldPos.x + fieldSize.x * 0.5f;
	cube->position.z = fieldPos.z + fieldSize.y * 0.5f;
	cube->position.y += cube->scale.y * 0.5f;
	//
	cube->isActive = false;
}

void Field::AlertToDanger()
{
	if (state != MapState::ALERT) return;
	//
	_alertTime += Timer::Get().GetElapsedTime();
	if (_alertTime >= 10.0f)
	{
		state = MapState::DANGER;
		SetState();
	}
}
