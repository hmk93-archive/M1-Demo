#include "pch.h"
#include "ModelClip.h"

ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
	for (auto frame : keyFrames)
		delete frame.second;
}

KeyFrame* ModelClip::GetKeyFrame(string name)
{
	if (keyFrames.count(name) == 0)
		return nullptr;

	return keyFrames[name];
}
