#include "pch.h"
#include "Game.h"
#include "Device.h"
#include "Environment.h"
#include "ConstBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Control.h"
#include "Timer.h"
#include "Font.h"

bool Game::s_exit = false;

#include "Cube.h"
#include "Sphere.h"
#include "ModelExporter.h"
#include "ModelObject.h"
#include "Player.h"

Game::Game()
{
	Device::Get();
	// Font::Get().Add();

	//_cube = new Cube();
	//_cube->position = Vector3(0.0f, 0.0f, 2.0f);

	//_sphere = new Sphere();
	//_sphere->position = Vector3(-2.0f, 0.0f, 2.0f);

	{
		// Export Model
		 //ModelExporter exporter("../Assets/Models/Paladin.fbx");
		 //exporter.ExportMaterial("Paladin/Paladin");
		 //exporter.ExportMesh("Paladin/Paladin");
	}
	{
		// Export Clip
		ModelExporter exporter("../Assets/Animations/Paladin/Idle.fbx");
		exporter.ExportClip("Paladin/Idle");
	}

	//_modelObj = new ModelObject("Paladin/Paladin");
	//_modelObj->position = Vector3(2.0f, 0.0f, 2.0f);
	//_modelObj->scale = Vector3(0.01f);

	_player = new Player("Paladin");
	_player->position = Vector3(0.0f, -1.0f, -5.0f);
	_player->scale = Vector3(0.01f);
}

Game::~Game()
{
	delete _player;
	delete _modelObj;
	delete _sphere;
	delete _cube;
	Texture::Delete();
	Shader::Delete();
}

void Game::Update()
{
	Control::Get().Update();
	Timer::Get().Update();

	// _cube->rotation.y += Timer::Get().GetElapsedTime();
	// _cube->Update();
	// _sphere->Update();
	// _modelObj->Update();
	_player->Update();
}

void Game::PreRender()
{
}

void Game::Render()
{
	Device::Get().SetRenderTarget();
	Environment::Get().Set();

	// _cube->Render();
	// _sphere->Render();
	// _modelObj->Render();
	_player->Render();
}

void Game::PostRender()
{
	// Font::Get().GetDC()->BeginDraw();
	// RenderFPS();
	// Font::Get().GetDC()->EndDraw();
}

void Game::RenderFPS()
{
	wstring fps = L"FPS: " + to_wstring((int)Timer::Get().GetFPS());
	Font::Get().RenderText(fps, { 10,0,0 });
}
