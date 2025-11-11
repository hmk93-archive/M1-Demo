#pragma once

#include "Transform.h"

class Collider;
class RasterizerState;

class FieldWall : public Transform
{
	using VertexType =  VertexUV;
public:
	FieldWall(Vector2 size, Vector2 layout = { 1.0f, 1.0f }, wstring textureFile = L"Rock");
	~FieldWall();

	void Update();
	void Render();

	Collider* GetCollider() { return _collider; }

private:
	void CreateMesh();
	void CreateMaterial(wstring textureFile);

private:
	Vector2 _size = Vector2::Zero;
	Vector2 _layout = Vector2::Zero;
	
	vector<VertexType> _vertices = {};
	vector<UINT> _indices = {};
	
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;
	
	RasterizerState* _rsState[2] = {};

	Collider* _collider = nullptr;
};