#pragma once

#include "ModelReader.h"

class FrameBuffer;
struct ClipTransform;
class ModelClip;

typedef function<void()> CallBack;
typedef function<void(int)> CallBackParam;

class ModelAnimator : public ModelReader
{
public:
	ModelAnimator(string file);
	virtual ~ModelAnimator();

	void ReadClip(string file, bool loopValue = true);

	virtual void Update();
	virtual void Render();

	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);

private:
	void CreateClipTransform(UINT index);

	void CreateTexture();

protected:
	FrameBuffer* frameBuffer = nullptr;
	ClipTransform* clipTransform = nullptr;
	ClipTransform* nodeTransform = nullptr;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	vector<ModelClip*> clips;

	map<UINT, CallBack> EndEvent;
	map<UINT, CallBackParam> EndParamEvent;
	map<UINT, int> param;

	map<UINT, CallBack> FrameEvent;
	map<UINT, vector<UINT>> frames;
	UINT tempFrame = 0;

	bool isPlay = true;
};

