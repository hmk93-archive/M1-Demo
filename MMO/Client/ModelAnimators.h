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
	};

public:
	ModelAnimators(string file);
	~ModelAnimators();

	virtual void Update() override;
	virtual void Render() override;
	void PostRender();

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	Transform* AddTransform();

	void UpdateTransforms();

public:
	Vector3 worldMinBox, worldMaxBox;

private:
	InstanceData _instanceData[MAX_INSTANCE] = {};
	VertexBuffer* _instanceBuffer = nullptr;

	vector<Transform*> _transforms = {};

	UINT _drawCount = 0;

	Vector3 _minBox, _maxBox;
};

