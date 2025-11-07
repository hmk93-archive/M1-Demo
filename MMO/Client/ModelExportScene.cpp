#include "pch.h"
#include "ModelExportScene.h"
#include "ModelExporter.h"
#include "Player.h"
#include "Enemy.h"
#include "ModelObject.h"

ModelExportScene::ModelExportScene()
{
	// Player
	{
		CreateModel("Paladin");
		CreateAnimation("Paladin", "Idle");
		CreateAnimation("Paladin", "Run");
		CreateAnimation("Paladin", "Jump");
		CreateAnimation("Paladin", "Attack_A");
		CreateAnimation("Paladin", "Attack_B");
		CreateAnimation("Paladin", "Hit");
		_player = new Player("Paladin");
	}
	// Monster
	{
		// Warrok
		{
			CreateModel("Warrok");
			CreateAnimation("Warrok", "Idle");
			CreateAnimation("Warrok", "Run");
			CreateAnimation("Warrok", "Punch");
			CreateAnimation("Warrok", "Hit");
			CreateAnimation("Warrok", "Dead");
			_warrok = new Enemy("Warrok");
			for (float z = -10.0f; z < 10.0f; z++)
			{
				for (float x = -10.0f; x < 10.0f; x++)
				{
					Transform* transform = _warrok->AddTransform();
					// Tag??
					transform->position = Vector3(20 * x, 0.0f, 20 * z);
					transform->scale = Vector3(0.1f);
				}
			}
		}
	}

	// Export
	{
		CreateModel("Stone");
		CreateModel("House");
		CreateModel("Sword");
	}

	// Create ModelObject
	{
		// Stone
		{
			ModelObject* obj = new ModelObject("Stone/Stone");
			obj->rotation.z = XM_PI;
			_models.emplace_back(obj);
		}
		// House
		{
			ModelObject* obj = new ModelObject("House/House");
			obj->scale = Vector3(0.01f);
			obj->rotation.x = XM_PIDIV2;
			_models.emplace_back(obj);
		}
		// Sword
		{
			ModelObject* obj = new ModelObject("Sword/Sword");
			obj->scale = Vector3(0.05f);
			_models.emplace_back(obj);
		}
	}
}

ModelExportScene::~ModelExportScene()
{
	delete _player;
	delete _warrok;
	for (ModelObject* model : _models)
		delete model;
}

void ModelExportScene::Update()
{
	// _player->Update();
	// _warrok->Update();
	for (ModelObject* model : _models)
		model->Update();
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
	// _player->Render();
	// _warrok->Render();
	for (ModelObject* model : _models)
		model->Render();
}

void ModelExportScene::PostRender()
{
	// _player->PostRender();
	// _warrok->PostRender();
}

void ModelExportScene::CreateModel(string name, string ext)
{
	_exporter = new ModelExporter("../Assets/Models/" + name + "." + ext);
	string savePath = name + "/" + name;
	_exporter->ExportMaterial(savePath);
	_exporter->ExportMesh(savePath);
	delete _exporter;
}

void ModelExportScene::CreateAnimation(string name, string clip)
{
	_exporter = new ModelExporter("../Assets/Animations/" + name + "/" + clip + ".fbx");
	_exporter->ExportClip(name + "/" + clip);
	delete _exporter;
}
