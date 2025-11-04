#pragma once

class UIImage;
class Collider;
class Texture;
class MaterialBuffer;

class UIButton
{
public:
	enum ButtonState
	{
		None,
		Over,
		Press
	} btnState = None;

public:
	UIButton(string tag = "NoTag", Vector3 pos = {}, Vector3 size = { 100.0f, 100.0f, 0 }, wstring file = L"ButtonDefault");
	virtual ~UIButton();

	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	void SetLeftButtonEvent(CallBack callback) { LeftBtnEvent = callback; }
	void SetRightButtonEvent(CallBack callback) { RightBtnEvent = callback; }

private:
	void SetState();
	void SetColor();

public:
	bool isActive = true;

private:
	UIImage* _uiImage = nullptr;
	Collider* _collider = nullptr;
	Texture* _texture = nullptr;
	MaterialBuffer* _materialBuffer = nullptr;
	
	CallBack LeftBtnEvent = nullptr;
	CallBack RightBtnEvent = nullptr;
};

