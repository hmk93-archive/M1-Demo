#include "pch.h"
#include "Cursor.h"
#include "Circle.h"
#include "Utility.h"
#include "Input.h"
#include "Timer.h"
#include "Terrain.h"
using namespace Utility;

Cursor::Cursor()
{
	_circle = new Circle(L"Effect/Cursor", 2.5f);
	_circle->SetTarget(this);
}

Cursor::~Cursor()
{
	delete _circle;
}

void Cursor::Update()
{
	position = ScreenToWorldHeightZero();
	if (_terrain)
		position.y = _terrain->GetHeight(position);

	PlayAnimation();

	Animation();

	Transform::UpdateWorld();
}

void Cursor::Render()
{
	RenderCircle();
}

void Cursor::PlayAnimation()
{
	if (Input::Get().Down(VK_LBUTTON))
	{
		ResetAnimation();
		_isPlay = true;
		_circle->isActive = true;
	}
}

void Cursor::Animation()
{
	if (!_isPlay)
		return;
	_circle->Update();

	static float animSpeed = 4.0f;
	_circle->scale.x -= animSpeed * Timer::Get().GetElapsedTime();
	_circle->scale.y -= animSpeed * Timer::Get().GetElapsedTime();
	_circle->scale.z -= animSpeed * Timer::Get().GetElapsedTime();

	if (_circle->scale.x < 0.1f)
		ResetAnimation();
}

void Cursor::ResetAnimation()
{
	_isPlay = false;
	_circle->isActive = false;
	_circle->scale = { 1, 1, 1 };
}

void Cursor::RenderCircle()
{
	if (_isPlay == false)
		return;
	_circle->Render();
}
