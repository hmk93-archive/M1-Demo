#include "pch.h"
#include "FloatText.h"
#include "Font.h"
#include "Timer.h"
#include "Utility.h"
#include "Transform.h"
using namespace Utility;

void FloatText::Update()
{
	if (!isActive)
		return;

	Fade();
	Floating();
	Scaling();
}

void FloatText::Render()
{
	if (!isActive)
		return;

	Font::Get().RenderText(_text, _pos, _fontSize, _color);
}

void FloatText::Activate(bool activate)
{
	isActive = activate;

	_randomPos.x += (float)Random(-_fontSize, _fontSize) * 0.5f;
	_randomPos.y += (float)Random(-_fontSize, _fontSize) * 0.5f;

	_fontSize *= Random(0.8f, 1.2f);
}

void FloatText::SetWorldPos(Vector3 pos)
{
	_pos = WorldToScreen(pos);
}

void FloatText::Fade()
{
	if (_color.a < 0.0f)
		Reset();
	
	_color.a -= _fadeSpeed * Timer::Get().GetElapsedTime();
}

void FloatText::Floating()
{
	_floatValue -= _floatSpeed * Timer::Get().GetElapsedTime();
	if (_target)
		_pos = WorldToScreen(_target->GlobalPos());
	_pos.y = g_screenHeight - _pos.y;
	_pos += _randomPos;
	_pos.y += _floatValue;
}

void FloatText::Scaling()
{
	float scalingSpeed = -5.0f;
	_fontSize += scalingSpeed * Timer::Get().GetElapsedTime();
}

void FloatText::Reset()
{
	isActive = false;
	_text = L"";
	_pos = {};
	_floatValue = 0.0f;
	_color = DXGI_RGBA{ 1.0f, 1.0f, 1.0f, 1.0f };
	_target = nullptr;
}

