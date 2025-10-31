#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;

struct MaterialData
{
	string name;

	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 emissive;

	string diffuseMap;
	string specularMap;
	string normalMap;
};

struct MeshData
{
	string name;
	string materialName;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

struct NodeData
{
	int index;
	string name;
	int parent;
	Matrix transform;
};

struct BoneData
{
	string name;
	int index;
	Matrix offset;
};

struct VertexWeights
{
	UINT indices[4];
	float weights[4];

	VertexWeights() : indices{}, weights{}
	{
	}

	void Add(const UINT& index, const float& weight)
	{
		for (UINT i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.0f;

		for (UINT i = 0; i < 4; i++)
			sum += weights[i];

		for (UINT i = 0; i < 4; i++)
			weights[i] /= sum;
	}
};

struct KeyTransform
{
	float time;

	Vector3 scale;
	Quaternion rotation;
	Vector3 position;
};

struct KeyFrame
{
	string boneName;
	vector<KeyTransform> transforms;
};

struct ClipNode
{
	vector<KeyTransform> keyFrame;
	aiString name;
};

struct Clip
{
	string name;

	UINT frameCount;
	float tickPerSecond;
	float duration;

	vector<KeyFrame*> keyFrame;
};

struct ClipTransform
{
	Matrix** transform;

	ClipTransform()
	{
		transform = new Matrix * [MAX_FRAME_KEY];

		for (UINT i = 0; i < MAX_FRAME_KEY; i++)
			transform[i] = new Matrix[MAX_BONE];
	}

	~ClipTransform()
	{
		for (UINT i = 0; i < MAX_FRAME_KEY; i++)
			delete[] transform[i];

		delete[] transform;
	}
};