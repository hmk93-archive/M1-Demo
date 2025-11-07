#pragma once

#include "Transform.h"

class UIImage;

class HUDHPBar : public Transform
{
	friend class Device;
public:
	enum HPBarElement
	{
		Background,
		Body,
		Face
	};

	HUDHPBar();
	~HUDHPBar();

	void Update();
	void Render();
	void PostRender();

	void SetPos(Vector3 worldPos);
	void SetScale(HPBarElement ele, float value);
	void SetCurrentBodyScale();

private:
	void Create();
	void SyncPos();
	void AnimBody();

public:
	const Vector3 myScale = { 100, 2, 1 };
	bool isHit = false;

private:
	vector<UIImage*> _images = {};
	Vector3 _offset = { -50.0f, 160.0f, 0 };
	float _animTime = 0.5f;
	float _currentBodyScale = 0.0f;
};

