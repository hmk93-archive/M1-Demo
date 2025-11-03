#pragma once

#include "ModelReader.h"

class FrameBuffer;
struct ClipTransform;
class ModelClip;

class ModelAnimator : public ModelReader
{
public:
	ModelAnimator(string file);
	virtual ~ModelAnimator();

	void ReadClip(string file, bool loopValue = true);

	virtual void Update();
	virtual void Render();

	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	void SetEndEvent(UINT clip, CallBack Event) { EndEvent[clip] = Event; }
	void SetEndParamEvent(UINT clip, CallBackParam Event) { EndParamEvent[clip] = Event; }
	void SetParam(UINT clip, int value) { param[clip] = value; }
	void SetFrameEvent(UINT clip, CallBack Event, vector<UINT> value);

	Matrix GetTransformByNode(int nodeIndex);
	Matrix GetTransformByNode(UINT instance, int nodeIndex);

protected:
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

