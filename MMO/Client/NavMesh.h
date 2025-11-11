#pragma once

#include "Transform.h"

class Terrain;
class Mesh;
class Material;
class RasterizerState;

// Experimental Class
class NavMesh : public Transform
{
public:
	NavMesh(UINT width, UINT height, bool debug = false);
	~NavMesh();

	void Update();
	void Render();

	void Bake();

	float GetHeightAt(Vector3 pos);
	bool IsWalkable(Vector3 pos);

	void AppendMesh(const vector<Vector3>& vertices, const vector<UINT>& indices);

private:
	void InitNavMesh();
	void CreateMesh();

private:
	UINT _width = 0;
	UINT _height = 0;

	vector<Vector3> _navVertices = {};
	vector<UINT> _navIndices = {};

	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

	RasterizerState* _fillModes[2] = {};

	rcContext _ctx = {};
	rcConfig _cfg = {};
	rcHeightfield* _solid = nullptr;
	rcCompactHeightfield* _chf = nullptr;
	rcPolyMesh* _pmesh = nullptr;

	vector<Vertex> _renderVertices = {};
	vector<UINT> _renderIndices = {};
};

