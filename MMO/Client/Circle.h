#pragma once

#include "Transform.h"

class Mesh;
class Material;
class MaterialBuffer;
class BlendState;

class Circle : public Transform
{
	using VertexType = VertexUV;

public:
	Circle(wstring textureFile, float radius = 1.0f, float angle = XM_2PI, UINT sliceCount = 20, wstring = L"Texture");
	~Circle();

	void Update();
	void Render();

	Material* GetMaterial() { return material; }
	float GetRadius() { return radius; }
	float GetAngle() { return angle; }

	void SetColor(Vector4 color);

private:
	void CreateMesh();

private:
	Mesh* mesh;
	Material* material;
	MaterialBuffer* materialBuffer;
	vector<VertexType> vertices;
	vector<UINT> indices;
	UINT sliceCount;
	float radius;
	float angle;
	BlendState* blendState[2];
};

