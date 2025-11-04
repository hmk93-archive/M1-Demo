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

	struct RegionContour
	{
		vector<Vector2> outer;
		vector<vector<Vector2>> holes;
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
	void SimplifyContours();
	void ClassifyHoles();
	void TriangulateRegion(int regionID, RegionContour& region);
	vector<Triangle> TriangulateContour(int regionID, const vector<Vector2>& contour);

	void CreateMesh();

	bool IsBoundaryCell(int x, int z, int regionID);
	bool CheckPointInTriangle(Vector3 pos, Triangle tri);
	bool IsPointInPolygon(Vector2 point, vector<Vector2>& polygon);

private:
	const float CELL_SIZE = 1.0f;
	const float MAX_STEP_HEIGHT = 0.5f;

	UINT _gridWidth = 0;
	UINT _gridHeight = 0;

	Terrain* _terrain = nullptr;

public:
	vector<vector<float>> _heightMap = {};
	vector<vector<bool>> _walkableMap = {};
	vector<vector<int>> _regionMap = {};
	vector<vector<Vector2>> _contours = {};
	
	vector<vector<Vector2>> _simpleContours = {};

	vector<RegionContour> _regionContours = {};

	using TriangleList = vector<Triangle>;
	map<int, TriangleList> _regionIdToTriList = {};

	vector<Vertex> _vertices = {};
	vector<UINT> _indices = {};

	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

	RasterizerState* _fillModes[2] = {};
};

