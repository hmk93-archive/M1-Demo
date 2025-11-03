#pragma once

#include "Scene.h"

class UIImage;
class UIButton;

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	UIImage* _background = nullptr;
	
	vector<UIButton*> _buttons = {};
};

