#include "pch.h"
#include "MapEditorScene.h"
#include "TerrainEditor.h"
#include "ModelObject.h"

MapEditorScene::MapEditorScene()
{
	_terrainEditor = new TerrainEditor(300, 300);

	// Create ModelObject
	{
		ModelObject* obj = new ModelObject("Stone/Stone");
		obj->rotation.z = XM_PI;
		_models.emplace_back(obj);
	}
}

MapEditorScene::~MapEditorScene()
{
	delete _terrainEditor;
	for (ModelObject* model : _models)
		delete model;
}

void MapEditorScene::Update()
{
	_terrainEditor->Update();
	for (ModelObject* model : _models)
		model->Update();
}

void MapEditorScene::Render()
{
	_terrainEditor->Render();
	for (ModelObject* model : _models)
		model->Render();
}

void MapEditorScene::PostRender()
{
	_terrainEditor->PostRender();
	for (ModelObject* model : _models)
		model->PostRender();
}