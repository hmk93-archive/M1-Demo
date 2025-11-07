#include "pch.h"
#include "UIManager.h"
#include "FloatText.h"

UIManager::UIManager()
{
	CreateFloatTexts();
}

UIManager::~UIManager()
{
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

void UIManager::InitFloatText(Transform* target, wstring text, DXGI_RGBA color)
{
	FloatText* floatText = SetFloatText();
	floatText->SetTarget(target);
	floatText->SetText(text);
	floatText->SetStartColor(color);
	floatText->Activate(true);
}
