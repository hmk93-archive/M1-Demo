#pragma once

#include "Scene.h"

class TerrainEditor;
class ModelObject;

class MapEditorScene : public Scene
{
public:
	MapEditorScene();
	~MapEditorScene();

	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;

private:
	TerrainEditor* _terrainEditor = nullptr;
	vector<ModelObject*> _models = {};
};

