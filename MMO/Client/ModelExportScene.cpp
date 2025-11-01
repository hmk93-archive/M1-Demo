#include "pch.h"
#include "ModelExportScene.h"
#include "ModelExporter.h"
#include "Player.h"
#include "Warrok.h"

ModelExportScene::ModelExportScene()
{
	// Player
	{
		CreateModel("Paladin");
		_player = new Player("Paladin");
	}

	// Monster
	{
		// Warrok
		{
			CreateModel("Warrok");
			CreateAnimation("Warrok", "Idle");
			_warrok = new Warrok("Warrok");
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
}

ModelExportScene::~ModelExportScene()
{
	delete _player;
	delete _warrok;
}

void ModelExportScene::Update()
{
	_player->Update();
	_warrok->Update();
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
	_player->Render();
	_warrok->Render();
}

void ModelExportScene::PostRender()
{
	_player->PostRender();
	_warrok->PostRender();
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
