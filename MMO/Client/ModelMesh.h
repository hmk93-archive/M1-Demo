#pragma once

#include "ModelType.h"

class Material;
class Mesh;

class ModelMesh
{
	friend class ModelReader;
public:
	ModelMesh();
	~ModelMesh();

	void CreateMesh();
	void Render();
	void Render(UINT drawCount);

	void SetBox(Vector3* minBox, Vector3* maxBox);

	ModelVertex* GetVertices() { return vertices; }
	UINT GetVertexCount() { return vertexCount; }
	UINT* GetIndices() { return indices; }
	UINT GetIndexCount() { return indexCount; }

	string name;
	string materialName;

private:
	Material* material;
	Mesh* mesh;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount;
	UINT indexCount;
};

