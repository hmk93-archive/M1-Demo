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
	void InitFileSystem();
	void FileSystem();
	void SaveFile();
	void LoadFile();

	void SaveMap(string path);
	void LoadMap(string path);

	void Control();

	void SetCamera();

private:
	TerrainEditor* _terrainEditor = nullptr;
	vector<ModelObject*> _models = {};
	ModelObject* _pickingModelCache = nullptr;
	string _projectPath = "";
	string _path = "";
};

