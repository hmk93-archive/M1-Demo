#pragma once

struct KeyFrame;

class ModelClip
{
	friend class ModelAnimator;
	friend class ModelAnimators;
public:
	ModelClip();
	~ModelClip();

	KeyFrame* GetKeyFrame(string name);

	string name = "";

	float duration = 0.0f;
	float tickPerSecond = 0.0f;
	UINT frameCount = 0;

	unordered_map<string, KeyFrame*> keyFrames = {};

	bool loop = true;
};