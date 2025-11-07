#pragma once

class Transform;

class FloatText
{
public:
	FloatText() = default;
	~FloatText() = default;

	void Update();
	void Render();

	void Activate(bool activate);
	
	void SetText(wstring value) { _text = value; }
	void SetTarget(Transform* target) { _target = target; }
	void SetStartColor(DXGI_RGBA color) { _color = color; }

	void SetWorldPos(Vector3 pos);
	void SetScreenPos(Vector3 pos) { _pos = pos; }

private:
	void Fade();
	void Floating();
	void Scaling();
	void Reset();

public:
	bool isActive = false;
private:
	Vector3 _pos = Vector3::Zero;
	Vector3 _randomPos = Vector3::Zero;
	wstring _text = L"";
	float _fontSize = 50.0f;
	DXGI_RGBA _color = DXGI_RGBA{ 1.0f, 1.0f, 1.0f, 1.0f };

	float _fadeTime = 2.0f;
	float _fadeSpeed = 1.0f / _fadeTime;
	float _floatSpeed = 20.0f;
	float _floatValue = 0.0f;

	Transform* _target = nullptr;
};

