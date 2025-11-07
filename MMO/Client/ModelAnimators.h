#pragma once

#include "ModelAnimator.h"

class VertexBuffer;
class Transform;

class ModelAnimators : public ModelAnimator
{
	struct InstanceData
	{
		Matrix world;
		UINT index;
		UINT instanceID;
	};

public:
	ModelAnimators(string file);
	virtual ~ModelAnimators();

	virtual void Update() override;
	virtual void Render() override;
	void PostRender();

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	void SetEndEvents(UINT instance, UINT clip, CallBackParam Event) { EndEvents[instance][clip] = Event; }
	void SetParams(UINT instance, UINT clip, int param) { params[instance][clip] = param; }

	void SetFrameEvents(UINT instanceID, UINT clip, CallBack Event, vector<UINT> value);

	Transform* AddTransform();

	void UpdateTransforms();

	UINT GetInstanceCount() { return (UINT)_transforms.size(); }

public:
	Vector3 worldMinBox, worldMaxBox;

protected:
	UINT _drawCount = 0;
	vector<Transform*> _transforms = {};
	InstanceData _instanceData[MAX_INSTANCE] = {};

private:
	VertexBuffer* _instanceBuffer = nullptr;

	Vector3 _minBox, _maxBox;

	vector<map<UINT, CallBackParam>> EndEvents;
	vector<map<UINT, int>> params;

	vector<map<UINT, CallBack>> FrameEvents;
	vector<map<UINT, vector<UINT>>> instFrames;
};

