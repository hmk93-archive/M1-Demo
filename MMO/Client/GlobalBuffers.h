#pragma once

#include "ConstBuffer.h"
#include "Light.h"

class MatrixBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	} data;

public:
	MatrixBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.matrix = Matrix::Identity;
	}

	void Set(Matrix value)
	{
		data.matrix = value.Transpose();
	}
};

class TypeBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int values[4];
		Data() : values{} {}
	} data;

	TypeBuffer() : ConstBuffer(&data, sizeof(Data)) {}
};

class BoneBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Matrix bones[MAX_BONE];
	} data;

	BoneBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (UINT i = 0; i < MAX_BONE; i++)
			data.bones[i] = Matrix::Identity;
	}

	void Add(Matrix matrix, UINT index)
	{
		data.bones[index] = matrix.Transpose();
	}
};

class FrameBuffer : public ConstBuffer
{
public:
	struct KeyFrameDesc
	{
		int clip = 0;
		UINT curFrame = 0;
		UINT nextFrame = 0;
		float time = 0.0f;

		float runningTime = 0.0f;
		float speed = 1.0f;
		float padding[2];
	};

	struct TweenDesc
	{
		float takeTime;
		float tweenTime;
		float runningTime;
		float padding;

		KeyFrameDesc cur;
		KeyFrameDesc next;

		TweenDesc() : takeTime(0.5f), tweenTime(0.5f), runningTime(0.0f)
		{
			cur.clip = 0;
			next.clip = -1;
		}
	};

	struct Data
	{
		TweenDesc tweenDesc[MAX_INSTANCE];
	} data;

	FrameBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

#define MAX_LIGHT 10
class Light;

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Light lights[MAX_LIGHT];
		UINT lightCount;
		float padding[3];
		
		Vector4 ambient;
		Vector4 ambientCeil;

	} data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.lightCount = 0;
		data.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		data.ambientCeil = { 0.1f, 0.1f, 0.1f, 1.0f };
	}

	void Add(Light light)
	{
		data.lights[data.lightCount++] = light;
	}

	void Add()
	{
		data.lightCount++;
	}
};

class RayBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Vector3 position;
		float size;

		Vector3 direction;
		float padding;
	} data;

	RayBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.position = Vector3(0, 0, 0);
		data.size = 0.0f;
		data.direction = Vector3(0, 0, 0);
	}
};