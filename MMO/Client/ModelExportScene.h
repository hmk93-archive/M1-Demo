#pragma once

#include "Scene.h"

class ModelExporter;
class Player;
class Enemy;
class ModelObject;

class ModelExportScene : public Scene
{
public:
	ModelExportScene();
	~ModelExportScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	
private:
	void CreateModel(string name, string ext = "fbx");
	void CreateAnimation(string name, string clip);

private:
	ModelExporter* _exporter = nullptr;

	Player* _player = nullptr;
	Enemy* _warrok = nullptr;

	vector<ModelObject*> _models = {};
};

