#pragma once

class Game
{
public:
	Game();
	~Game();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	void RenderFPS();
	void Debug();
	void UpdateGuizmo();

public:
	static bool s_exit;
};

