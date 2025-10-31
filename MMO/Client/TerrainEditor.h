#pragma once

#include "Transform.h"
#include "GlobalBuffers.h"

class Material;
class Mesh;
class Texture;
class ComputeShader;
class StructuredBuffer;
class RayBuffer;

class TerrainEditor : public Transform
{
	using VertexType = VertexUVNormalTangentAlpha;

	class BrushBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int type;
			Vector3 location;
			float range;
			Vector3 color;
		} data;

		BrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.location = Vector3(0.0f);
			data.color = Vector3(0.0f, 0.5f, 0.0f);
			data.type = 0;
			data.range = 10.0f;
		}
	};

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
	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRender();

private:
	void CreateMesh();
	void CreateCompute();

	void SetInput();

	bool ComputePicking(OUT Vector3& position);

	void AdjustY(Vector3 position);

	void InitFileSystem();
	void FileSystem();
	void SaveFile();
	void LoadFile();

	void SaveHeight(wstring heightFile);
	void LoadHeight(wstring heightFile);

private:
	const float MAX_HEIGHT = 30.0f;
	
	float _adjustValue = 50.0f;

	bool _isRaise = true;

	Material* _material = nullptr;
	Mesh* _mesh;

	Texture* _heightMap = nullptr;
	
	Texture* _alphaMap = nullptr;
	Texture* _secondMap = nullptr;
	Texture* _thirdMap = nullptr;

	BrushBuffer* _brushBuffer = nullptr;

	vector<VertexType> _vertices;
	vector<UINT> _indices;

	UINT _width = 0;
	UINT _height = 0;

	RayBuffer* _rayBuffer = nullptr;

	ComputeShader* _computeShader = nullptr;
	StructuredBuffer* _structuredBuffer = nullptr;
	InputDesc* _input = nullptr;
	OutputDesc* _output = nullptr;

	UINT _computeSize = 0;

	// Filesystem
	string _projectPath = "";
	string _path = "";
};

