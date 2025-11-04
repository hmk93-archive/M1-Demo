#include "pch.h"
#include "UIButton.h"
#include "Texture.h"
#include "UIImage.h"
#include "Material.h"
#include "SquareCollider.h"
#include "Input.h"

UIButton::UIButton(string tag, Vector3 pos, Vector3 size, wstring file)
{
	wstring filePath = L"../Assets/Textures/" + file + L".png";
	_texture = Texture::Add(filePath);

	_uiImage = new UIImage(L"Texture");
	_uiImage->position = pos;
	_uiImage->scale = size;
	_uiImage->SetSRV(_texture->GetSRV());

	_materialBuffer = _uiImage->GetMaterial()->GetBuffer();

	Vector2 halfSize = { _uiImage->scale.x * 0.5f, _uiImage->scale.y * 0.5f };
	Vector4 square = { -halfSize.x, halfSize.y, halfSize.x , -halfSize.y };
	_collider = new SquareCollider(square);
	_collider->SetTarget(_uiImage);
}

UIButton::~UIButton()
{
	delete _collider;
	delete _uiImage;
}

void UIButton::Update()
{
	if (isActive == false)
		return;

	SetState();
	SetColor();
}

void UIButton::Render()
{
	if (isActive == false)
		return;
	_uiImage->Render();
	_collider->Render();
}

void UIButton::PostRender()
{
	if (isActive == false)
		return;
}

void UIButton::SetState()
{
	if (_collider->MouseCollision())
	{
		btnState = Over;
		if (Input::Get().Press(VK_LBUTTON) || Input::Get().Press(VK_RBUTTON))
			btnState = Press;
		if (Input::Get().Up(VK_LBUTTON))
			if (LeftBtnEvent)
				LeftBtnEvent();
		if (Input::Get().Up(VK_RBUTTON))
			if (RightBtnEvent)
				RightBtnEvent();
	}
	else
	{
		btnState = None;
	}
}

void UIButton::SetColor()
{
	switch (btnState)
	{
	case None:
		_collider->SetColor(Vector4(0, 1.0f, 0, 1.0f));
		_materialBuffer->data.diffuse = Vector4(1.0f);
		break;
	case Over:
		_collider->SetColor(Vector4(1.0f, 0.0f, 0, 1.0f));
		_materialBuffer->data.diffuse = Vector4(0.7f, 0.7f, 0.9f, 1.0f);
		break;
	case Press:
		_collider->SetColor(Vector4(0, 0, 1.0f, 1.0f));
		_materialBuffer->data.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	}
}
