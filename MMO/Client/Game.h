#pragma once

class PostProcess;

class Game
{
public:
	Game();
	~Game();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

public:
	static void InitPostProcess();

private:
	void RenderFPS();
	void Debug();
	void UpdateGuizmo();

public:
	static bool s_exit;
	static PostProcess* s_postProcess;
};

