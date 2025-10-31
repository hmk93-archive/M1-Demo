#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
	for (auto scene : _scenes)
		delete scene.second;
}

void SceneManager::Update()
{
	for (Scene* scene : _playScene)
		scene->Update();
}

void SceneManager::PreRender()
{
	for (Scene* scene : _playScene)
		scene->PreRender();
}

void SceneManager::Render()
{
	for (Scene* scene : _playScene)
		scene->Render();
}

void SceneManager::PostRender()
{
	for (Scene* scene : _playScene)
		scene->PostRender();
}

Scene* SceneManager::Add(string key, Scene* scene)
{
	if (_scenes.count(key) > 0)
	{
		return _scenes[key];
	}
	else
	{
		_scenes[key] = scene;
		return scene;
	}
}

Scene* SceneManager::Play(string key)
{
	_playScene.emplace_back(_scenes[key]);
	return _scenes[key];
}

void SceneManager::Delete(string key)
{
	vector<Scene*>::iterator iter = _playScene.begin();

	while (iter != _playScene.end())
	{
		if ((*iter) == _scenes[key])
		{
			iter = _playScene.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}