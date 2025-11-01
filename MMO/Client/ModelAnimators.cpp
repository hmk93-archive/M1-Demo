#include "pch.h"
#include "ModelAnimators.h"
#include "VertexBuffer.h"
#include "GlobalBuffers.h"
#include "ModelClip.h"
#include "Timer.h"
#include "Transform.h"
#include "Device.h"

ModelAnimators::ModelAnimators(string file)
	: ModelAnimator(file)
	, _minBox{}
	, _maxBox{}
	, worldMinBox{}
	, worldMaxBox{}
{
	for (UINT i = 0; i < MAX_INSTANCE; i++)
	{
		_instanceData[i].world = Matrix::Identity;
		_instanceData[i].index = i;
	}

	_instanceBuffer = new VertexBuffer(_instanceData, sizeof(InstanceData), MAX_INSTANCE);

	SetBox(&_minBox, &_maxBox);
}

ModelAnimators::~ModelAnimators()
{
	for (Transform* transform : _transforms)
		delete transform;
	delete _instanceBuffer;
}

void ModelAnimators::Update()
{
	for (UINT i = 0; i < _transforms.size(); i++)
	{
		FrameBuffer::TweenDesc& tweenDesc = frameBuffer->data.tweenDesc[i];

		{
			FrameBuffer::KeyFrameDesc& desc = tweenDesc.cur;
			ModelClip* clip = clips[desc.clip];

			float time = 1.0f / clip->tickPerSecond / desc.speed;
			desc.runningTime += Timer::Get().GetElapsedTime();

			if (desc.time >= 1.0f)
			{
				if (desc.curFrame + desc.time >= clip->frameCount)
				{
					//if (EndEvents[i].count(desc.clip) > 0)
					//	EndEvents[i][desc.clip](params[i][desc.clip]);
				}

				desc.curFrame = (desc.curFrame + 1) % clip->frameCount;
				desc.nextFrame = (desc.curFrame + 1) % clip->frameCount;
				desc.runningTime = 0.0f;
			}

			desc.time = desc.runningTime / time;
		}

		{
			FrameBuffer::KeyFrameDesc& desc = tweenDesc.next;

			if (desc.clip > -1)
			{
				ModelClip* clip = clips[desc.clip];

				tweenDesc.runningTime += Timer::Get().GetElapsedTime();
				tweenDesc.tweenTime = tweenDesc.runningTime / tweenDesc.takeTime;

				if (tweenDesc.tweenTime >= 1.0f)
				{
					tweenDesc.cur = desc;
					tweenDesc.runningTime = 0.0f;
					tweenDesc.tweenTime = 0.0f;

					desc.runningTime = 0.0f;
					desc.curFrame = 0;
					desc.nextFrame = 0;
					desc.time = 0.0f;
					desc.clip = -1;
				}
				else
				{
					float time = 1.0f / clip->tickPerSecond / desc.speed;
					desc.runningTime += Timer::Get().GetElapsedTime();;

					if (desc.time >= 1.0f)
					{
						desc.curFrame = (desc.curFrame + 1) % clip->frameCount;
						desc.nextFrame = (desc.curFrame + 1) % clip->frameCount;
						desc.runningTime = 0.0f;
					}

					desc.time = desc.runningTime / time;
				}
			}
		}
	}

	UpdateTransforms();
}

void ModelAnimators::Render()
{
	if (texture == nullptr)
		CreateTexture();

	frameBuffer->SetVSBuffer(4);
	Device::Get().GetDeviceContext()->VSSetShaderResources(0, 1, &srv);

	_instanceBuffer->IASet(1);

	MeshRender(_drawCount);
}

void ModelAnimators::PostRender()
{

}

void ModelAnimators::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	frameBuffer->data.tweenDesc[instance].takeTime = takeTime;
	frameBuffer->data.tweenDesc[instance].next.clip = clip;
	frameBuffer->data.tweenDesc[instance].next.speed = speed;
}

Transform* ModelAnimators::AddTransform()
{
	Transform* transform = new Transform();
	_transforms.emplace_back(transform);
	return transform;
}

void ModelAnimators::UpdateTransforms()
{
	_drawCount = 0;
	for (UINT i = 0; i < _transforms.size(); i++)
	{
		_transforms[i]->UpdateWorld();
		worldMinBox = Vector3::Transform(_minBox, _transforms[i]->GetWorld());
		worldMaxBox = Vector3::Transform(_maxBox, _transforms[i]->GetWorld());

		_instanceData[_drawCount].world = _transforms[i]->GetWorld().Transpose();
		_instanceData[_drawCount].index = i;
		_drawCount++;
	}

	_instanceBuffer->Update(_instanceData, _drawCount);
}
