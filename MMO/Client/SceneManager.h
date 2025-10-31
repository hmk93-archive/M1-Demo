#pragma once

class Scene;

class SceneManager
{
	DECLARE_SINGLETON(SceneManager);
public:
	void Update();
	void PreRender();
	void Render();
	void PostRender();

	Scene* Add(string key, Scene* scene);

	Scene* Play(string key);

	void Delete(string key);

	Scene* GetCurrentScene() { return _playScene[0]; }

private:
	map<string, Scene*> _scenes;
	vector<Scene*> _playScene;
};

