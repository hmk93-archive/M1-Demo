#include "pch.h"
#include "UIPointBar.h"
#include "Texture.h"
#include "UIImage.h"

UIPointBar::UIPointBar(wstring textureFile, Vector3 halfSize, Vector3 pos)
	: _halfSize(halfSize)
{
	Create(textureFile, pos);
}

UIPointBar::~UIPointBar()
{
	for (UIImage* image : _images)
		delete image;
	_images.clear();
}

void UIPointBar::Update()
{

}

void UIPointBar::Render()
{
	for (UIImage* image : _images)
		image->Render();
}

void UIPointBar::PostRender()
{

}

void UIPointBar::Create(wstring textureFile, Vector3 pos)
{
	Texture* texture = nullptr;
	UIImage* image = nullptr;

	//texture = Texture::Add(L"../Assets/Textures/UI/PointBar" + textureFile + L"Face" + L".png");
	//image = new UIImage(L"Texture", UIImage::Pivot::Center, true);
	//image->SetSRV(texture->GetSRV());
	//image->scale = _halfSize * 2.0f;
	//image->position = pos;
	//_images.emplace_back(image);

	texture = Texture::Add(L"../Assets/Textures/UI/PointbarBackground.png");
	image = new UIImage(L"Texture", UIImage::Pivot::Center, true);
	image->SetSRV(texture->GetSRV());
	image->scale = _halfSize * 2.0f;
	image->position = pos;
	_images.emplace_back(image);
}
