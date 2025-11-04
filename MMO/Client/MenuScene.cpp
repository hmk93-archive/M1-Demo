#include "pch.h"
#include "MenuScene.h"
#include "Texture.h"
#include "UIImage.h"
#include "UIButton.h"
#include "SceneManager.h"

MenuScene::MenuScene()
{
	Texture* texture = nullptr;
	wstring path = L"../Assets/Textures/Background.png";
	texture = Texture::Add(path);
	_background = new UIImage(L"Texture");
	_background->SetSRV(texture->GetSRV());
	_background->scale = { (float)g_screenWidth, (float)g_screenHeight, 0 };
	_background->position = { g_screenWidth * 0.5f, g_screenHeight * 0.5f, 0 };

	UIButton* button = nullptr;
	
	const float offset = 5.0f;
	button = new UIButton("GameStartButton", Vector3(g_screenWidth * 0.5f, g_screenHeight * 0.5f, 0), Vector3(200.0f, 80.0f, 0), L"GameStartButton");
	button->SetLeftButtonEvent(bind(&MenuScene::GameStartButton, this));
	_buttons.emplace_back(button);
	button = new UIButton("MapEitdorButton", Vector3(g_screenWidth * 0.5f, g_screenHeight * 0.5f - 80.0f - offset, 0), Vector3(200.0f, 80.0f, 0), L"MapEditorButton");
	button->SetLeftButtonEvent(bind(&MenuScene::MapEditorButton, this));
	_buttons.emplace_back(button);
}

MenuScene::~MenuScene()
{
	for (UIButton* button : _buttons)
		delete button;
	delete _background;
}

void MenuScene::Update()
{
	_background->Update();
	for (UIButton* button : _buttons)
		button->Update();
}

void MenuScene::PreRender()
{

}

void MenuScene::Render()
{
;
}

void MenuScene::PostRender()
{
	_background->Render();
	for (UIButton* button : _buttons)
		button->Render();
}

void MenuScene::GameStartButton()
{
	SceneManager::Get().Delete("Menu");
	SceneManager::Get().Play("InGame");
}

void MenuScene::MapEditorButton()
{
	SceneManager::Get().Delete("Menu");
	SceneManager::Get().Play("MapEditor");
}
