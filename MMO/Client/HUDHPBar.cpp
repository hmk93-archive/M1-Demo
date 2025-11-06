#include "pch.h"
#include "HUDHPBar.h"
#include "UIImage.h"
#include "Texture.h"
#include "Timer.h"
#include "Utility.h"
using namespace Utility;

HUDHPBar::HUDHPBar()
{
	Create();
}

HUDHPBar::~HUDHPBar()
{
	for (UIImage* image : _images)
		delete image;
	_images.clear();
}

void HUDHPBar::Update()
{
	AnimBody();
	Transform::UpdateWorld();
	SyncPos();
}

void HUDHPBar::Render()
{
	for (UIImage* image : _images)
		image->Render();
}

void HUDHPBar::PostRender()
{

}

void HUDHPBar::SetPos(Vector3 worldPos)
{
	Vector3 screenPos = WorldToScreen(worldPos);
	position = screenPos;
	position.x += _offset.x;
	position.y += _offset.y;
	SyncPos();
}

void HUDHPBar::SetScale(HPBarElement ele, float value)
{
	_images[ele]->scale.x = value * myScale.x;
}

void HUDHPBar::SetCurrentBodyScale()
{
	_currentBodyScale = _images[Body]->scale.x;
}

void HUDHPBar::SyncPos()
{
	for (UIImage* image : _images)
		image->position = position;
}

void HUDHPBar::AnimBody()
{
	if (!isHit)
		return;

	static float time = 0.0f;
	time += (1.0f / _animTime) * Timer::Get().GetElapsedTime();

	float body = _images[Body]->scale.x;
	float face = _images[Face]->scale.x;

	if (body - face <= 0.0f)
	{
		isHit = false;
		time = 0.0f;
		_images[Body]->scale.x = face;
		return;
	}

	_images[Body]->scale.x = LERP(_currentBodyScale, face, time);
}

void HUDHPBar::Create()
{
	wstring comm = L"HUD";
	vector<wstring> file = { L"Background", L"Body", L"Face" };
	for (size_t i = 0; i < file.size(); ++i)
	{
		UIImage* ui = new UIImage(L"Texture", UIImage::LC);
		wstring path = L"../Assets/Textures/UI/" + comm + file[i] + L".png";
		Texture* image = Texture::Add(path);
		ui->SetSRV(image->GetSRV());
		ui->scale = myScale;
		_images.emplace_back(ui);
	}
}
