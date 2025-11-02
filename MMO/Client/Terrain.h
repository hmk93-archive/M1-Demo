#pragma once

#include "Transform.h"

class Material;
class Mesh;
class Texture;
class TypeBuffer;
class RayBuffer;
class ComputeShader;
class StructuredBuffer;

class Terrain : public Transform
{
	using VertexType = VertexUVNormalTangentAlpha;

	struct InputDesc
	{
		UINT index;
		Vector3 v0, v1, v2;
	};

	struct OutputDesc
	{
		int picked;
		float u, v, distance;
	};

public:
	Terrain(Vector3 pos = Vector3(0.0f));
	virtual ~Terrain();

	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	bool ComputePicking(OUT Vector3& position);

	float GetHeight(Vector3 position);
	Vector2 GetSize() { return Vector2((float)_width, (float)_height); }

private:
	void CreateMesh();
	void CreateCompute();

	void CreateInput();
	void CreateAlpha();

private:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	vector<VertexType> _vertices = {};
	vector<UINT> _indices = {};

	UINT _width = 0;
	UINT _height = 0;

	Texture* _heightMap = nullptr;
	Texture* _alphaMap[2] = {};
	Texture* _secondMap = nullptr;
	Texture* _thirdMap = nullptr;

	TypeBuffer* _typeBuffer = nullptr;

	RayBuffer* _rayBuffer = nullptr;
	ComputeShader* _computeShader = nullptr;
	StructuredBuffer* _structuredBuffer = nullptr;

	UINT _computeSize = 0;

	InputDesc* _input = nullptr;
	OutputDesc* _output = nullptr;
};

