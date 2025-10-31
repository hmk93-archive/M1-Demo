#pragma once

#include "Transform.h"

class Material;
class Mesh;

class Cube : public Transform
{
public:
	Cube(wstring file = L"Texture");
	~Cube();

	void Update();
	void Render();
	void PostRender();

private:
	void Create();
	void CreateNormal();

private:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	vector<VertexUVNormal> _vertices = {};
	vector<UINT> _indices = {};
};

