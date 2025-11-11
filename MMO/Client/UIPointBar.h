#pragma once

class UIImage;
class Player;

class UIPointBar
{
public:
	UIPointBar(wstring textureFile, Vector3 halfSize, Vector3 pos);
	~UIPointBar();

	void Update();
	void Render();
	void PostRender();

	void SetPlayer(Player* player) { _player = player; }

private:
	void Create(wstring textureFile, Vector3 pos);

private:
	vector<UIImage*> _images = {};

	Player* _player = nullptr;
	
	Vector3 _halfSize = Vector3::Zero;
};

