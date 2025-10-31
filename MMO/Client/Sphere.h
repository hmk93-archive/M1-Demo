#pragma once

#include "Transform.h"

class Material;
class Mesh;

class Sphere : public Transform
{
public:
	Sphere(wstring file = L"Texture", float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
	~Sphere();

	void Update();
	void Render();
	void PostRender();

	Material* GetMaterial() { return _material; }
	float GetRadius() { return _radius; }

private:
	void CreateMesh();
	void CreateTangent();

private:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	vector<VertexUVNormalTangent> _vertices = {};
	vector<UINT> _indices = {};

	UINT _sliceCount = 0;
	UINT _stackCount = 0;
	float _radius = 0.0f;
};

