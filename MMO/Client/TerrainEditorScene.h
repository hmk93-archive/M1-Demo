#pragma once

#include "Scene.h"

class TerrainEditor;

class TerrainEditorScene : public Scene
{
public:
	TerrainEditorScene();
	~TerrainEditorScene();

	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;

private:
	TerrainEditor* _terrainEditor = nullptr;
};

