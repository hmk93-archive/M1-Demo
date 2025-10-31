#include "pch.h"
#include "TerrainEditorScene.h"
#include "TerrainEditor.h"

TerrainEditorScene::TerrainEditorScene()
{
	_terrainEditor = new TerrainEditor(100, 100);
}

TerrainEditorScene::~TerrainEditorScene()
{
	delete _terrainEditor;
}

void TerrainEditorScene::Update()
{
	_terrainEditor->Update();
}

void TerrainEditorScene::Render()
{
	_terrainEditor->Render();
}

void TerrainEditorScene::PostRender()
{
	_terrainEditor->PostRender();
}