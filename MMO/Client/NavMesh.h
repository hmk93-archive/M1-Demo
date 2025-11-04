#pragma once

#include "Transform.h"

class Terrain;
class Mesh;
class Material;
class RasterizerState;

class NavMesh : public Transform
{
	struct Triangle
	{
		Vector3 v0, v1, v2;
	};
public:
	NavMesh(UINT width, UINT height);
	~NavMesh();

	void Update();
	void Render();

	void Bake();
	bool PointInTriangle(Vector3 pos);

	void SetTerrain(Terrain* terrain) { _terrain = terrain; }

private:
	void CreateHeightAndWalkableMap();
	void CreateRegionBFS();
	void CreateContours();
	void TriangulateContour(UINT regionID, const vector<Vector2>& contour);

	void CreateMesh();

	bool CheckPointInTriangle(Vector3 pos, Triangle tri);

private:
	const float CELL_SIZE = 1.0f;
	const float MAX_STEP_HEIGHT = 1.0f;

	UINT _gridWidth = 0;
	UINT _gridHeight = 0;

	Terrain* _terrain = nullptr;

public:
	vector<vector<float>> _heightMap = {};
	vector<vector<bool>> _walkableMap = {};
	vector<vector<int>> _regionMap = {};
	vector<vector<Vector2>> _contours = {};

	using TriangleList = vector<Triangle>;
	map<UINT, TriangleList> _regionIdToTriList = {};

	vector<Vertex> _vertices = {};
	vector<UINT> _indices = {};

	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

	RasterizerState* _fillModes[2] = {};
};

