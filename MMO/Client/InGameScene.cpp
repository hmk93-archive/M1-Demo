#include "pch.h"
#include "InGameScene.h"
#include "Terrain.h"
#include "Player.h"

InGameScene::InGameScene()
{
	_terrain = new Terrain();
	_player = new Player("Paladin");
	_player->position = Vector3(25.0f, 0.0f, 25.0f);
	_player->scale = Vector3(0.05f);
	_player->SetTerrain(_terrain);
}

InGameScene::~InGameScene()
{
	delete _player;
	delete _terrain;
}

void InGameScene::Update()
{
	_terrain->Update();
	_player->Update();
}

void InGameScene::PreRender()
{
}

void InGameScene::Render()
{
	_terrain->Render();
	_player->Render();
}

void InGameScene::PostRender()
{
	_terrain->PostRender();
	_player->PostRender();
}