#pragma once

class FloatText;
class Transform;

class UIManager
{
	DECLARE_SINGLETON(UIManager);
public:
	void Update();
	void Render();
	void PostRender();

	void InitFloatText(Transform* target, wstring text, DXGI_RGBA color = DXGI_RGBA{1.0f, 1.0f, 1.0f, 1.0f});

private:
	void CreateFloatTexts();

	FloatText* SetFloatText();

private:
	vector<FloatText*> _texts = {};
	UINT _floatTextIndex = 0;
};

