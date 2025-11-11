#include "pch.h"
#include "UIManager.h"
#include "FloatText.h"
#include "UIPointBar.h"

UIManager::UIManager()
{
	CreateStatus();
	CreateFloatTexts();
}

UIManager::~UIManager()
{
	delete _status;
	for (FloatText* text : _texts)
		delete text;
}

void UIManager::Update()
{
	for (FloatText* text : _texts)
		text->Update();
}

void UIManager::Render()
{
	_status->Render();
}

void UIManager::PostRender()
{
	for (FloatText* text : _texts)
		text->Render();
}

FloatText* UIManager::SetFloatText()
{
	FloatText* text = _texts[_floatTextIndex++];
	_floatTextIndex %= _texts.size();
	return text;
}

void UIManager::CreateFloatTexts()
{
	UINT textCount = 100;
	for (UINT i = 0; i < textCount; i++)
	{
		FloatText* text = new FloatText();
		_texts.push_back(text);
	}
}

void UIManager::CreateStatus()
{
	_status = new UIPointBar(L"Health", { 150, 20, 0 }, Vector3(g_screenWidth * 0.5f, 40.0f, 0.0f));
}

void UIManager::InitFloatText(Transform* target, wstring text, DXGI_RGBA color)
{
	FloatText* floatText = SetFloatText();
	floatText->SetTarget(target);
	floatText->SetText(text);
	floatText->SetStartColor(color);
	floatText->Activate(true);
}
