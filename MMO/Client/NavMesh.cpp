#include "pch.h"
#include "NavMesh.h"
#include "Terrain.h"
#include "Mesh.h"
#include "Material.h"
#include "Device.h"
#include "RasterizerState.h"

NavMesh::NavMesh(UINT width, UINT height)
{
	_material = new Material(L"Collider");
	_material->GetBuffer()->data.diffuse = Vector4(0, 1, 0, 1);

	_gridWidth = (UINT)((float)width / CELL_SIZE);
	_gridHeight = (UINT)((float)height / CELL_SIZE);

	_fillModes[0] = new RasterizerState();
	_fillModes[1] = new RasterizerState();
	_fillModes[1]->FillMode(D3D11_FILL_WIREFRAME);
}

NavMesh::~NavMesh()
{
	for (RasterizerState* fillMode : _fillModes)
		delete fillMode;
	delete _material;
	delete _mesh;
}

void NavMesh::Update()
{

}

void NavMesh::Render()
{
	if (isActive == false)
		return;

	SetWorldBuffer();

	_mesh->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_material->Set();

	_fillModes[1]->SetState();
	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
	_fillModes[0]->SetState();
}

void NavMesh::Bake()
{
	CreateHeightAndWalkableMap();
	cout << "Complete CreateHeightAndWalkableMap()" << endl;
	CreateRegionBFS();
	cout << "Complete CreateHeightAndWalkableMap()" << endl;
	CreateContours();
	cout << "Complete CreateHeightAndWalkableMap()" << endl;
	for (UINT i = 0; i < _contours.size(); i++)
	{
		TriangulateContour(i, _contours[i]);
		cout << "Complete TriangulateContour() RegionID:" << i << endl;
	}

	CreateMesh();
	cout << "Complete CreateMesh()" << endl;
}

bool NavMesh::PointInTriangle(Vector3 pos)
{
	for (auto& triList : _regionIdToTriList)
	{
		for (auto& tri : triList.second)
		{
			if (CheckPointInTriangle(pos, tri))
				return true;
		}
	}
	return false;
}

void NavMesh::CreateHeightAndWalkableMap()
{
	if (!_terrain)
		return;
	// Terrain
	_heightMap.resize(_gridHeight, vector<float>(_gridWidth, 0.0f));
	_walkableMap.resize(_gridHeight, vector<bool>(_gridWidth, false));
	for (UINT z = 0; z < _gridHeight; z++)
	{
		for (UINT x = 0; x < _gridWidth; x++)
		{
			float h = _terrain->GetHeight(Vector3(x, 0, z));
			bool walkable = true;
			_heightMap[z][x] = h;
			for (int dz = -1; dz <= 1; dz++)
			{
				for (int dx = -1; dx <= 1; dx++)
				{
					if (dx == 0 && dz == 0)
						continue;
					int nx = x + dx;
					int nz = z + dz;
					if (nx < 0 || nz < 0 || nx >= (int)_gridWidth || nz >= (int)_gridHeight)
						continue;
					float nh = _terrain->GetHeight(Vector3(nx, 0, nz));
					float delta = abs(nh - h);
					if (delta > MAX_STEP_HEIGHT)
					{
						walkable = false;
						break;
					}
				}
				if (!walkable)
					break;
			}
			_walkableMap[z][x] = walkable;
		}
	}
}

void NavMesh::CreateRegionBFS()
{
	if (_walkableMap.empty())
		return;
	int curRegionID = 0;
	int dx[] = { -1, 1, 0, 0 };
	int dz[] = { 0, 0, -1, 1 };
	_regionMap.resize(_gridHeight, vector<int>(_gridWidth, -1));
	for (UINT z = 0; z < _gridHeight; z++)
	{
		for (UINT x = 0; x < _gridWidth; x++)
		{
			if (_regionMap[z][x] != -1 || !_walkableMap[z][x])
				continue;
			// BFS
			queue<pair<int, int>> q;
			q.push(make_pair((int)x, (int)z));
			_regionMap[z][x] = curRegionID;
			while (!q.empty())
			{
				auto c = q.front();
				q.pop();
				for (int i = 0; i < 4; i++)
				{
					int nx = c.first + dx[i];
					int nz = c.second + dz[i];
					if (nx < 0 || nz < 0 || nx >= (int)_gridWidth || nz >= (int)_gridHeight)
						continue;
					if (!_walkableMap[nz][nx] || _regionMap[nz][nx] != -1)
						continue;
					_regionMap[nz][nx] = curRegionID;
					q.push(make_pair(nx, nz));
				}
			}
			curRegionID++;
		}
	}
}

void NavMesh::CreateContours()
{
	vector<vector<bool>> visited(_gridHeight, vector<bool>(_gridWidth, false));
	int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dz[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	for (UINT z = 0; z < _gridHeight; z++)
	{
		for (UINT x = 0; x < _gridWidth; x++)
		{
			int regionID = _regionMap[z][x];
			if (regionID == -1 || visited[z][x])
				continue;
			vector<Vector2> contour;
			Vector2 start = { (float)x, (float)z };
			Vector2 current = start;
			int dir = 0;
			do
			{
				contour.push_back(current);
				visited[current.y][current.x] = true;
				bool foundNext = false;
				for (int i = 0; i < 8; i++)
				{
					int ndir = (dir + i) % 8;
					int nx = (int)current.x + dx[ndir];
					int nz = (int)current.y + dz[ndir];
					if (nx < 0 || nz < 0 || nx >= (int)_gridWidth || nz >= (int)_gridHeight)
						continue;
					if (_regionMap[nz][nx] == regionID && !visited[nz][nx])
					{
						current = { (float)nx, (float)nz };
						dir = ndir;
						foundNext = true;
						break;
					}
				}
				if (!foundNext)
					break;
			} while (!(current.x == start.x && current.y == start.y));
			
			if (!contour.empty())
				_contours.push_back(contour);
		}
	}
}

float Cross(const Vector2& a, const Vector2& b, const Vector2& c)
{
	float abx = b.x - a.x;
	float aby = b.y - a.y;
	float acx = c.x - a.x;
	float acy = c.y - a.y;
	return abx * acy - aby * acx;
}

void NavMesh::TriangulateContour(UINT regionID, const vector<Vector2>& contour)
{
	vector<Triangle> triangles;
	if (contour.size() < 3)
		return;
	vector<UINT> indices(contour.size());
	for (UINT i = 0; i < contour.size(); i++)
		indices[i] = i;

	while (indices.size() >= 3)
	{
		bool earFound = false;
		UINT n = (UINT)indices.size();
		for (UINT i = 0; i < n; i++)
		{
			UINT i0 = indices[(i + n - 1) % n];
			UINT i1 = indices[i];
			UINT i2 = indices[(i + 1) % n];
			
			Vector2 v0 = contour[i0];
			Vector2 v1 = contour[i1];
			Vector2 v2 = contour[i2];

			float cross = Cross(v0, v1, v2);
			if (cross <= 0.0f)
				continue;
			//@TODO:

			Vector3 ta = { v0.x, _terrain->GetHeight(Vector3(v0.x, 0.0f, v0.y)), v0.y };
			Vector3 tb = { v1.x, _terrain->GetHeight(Vector3(v1.x, 0.0f, v1.y)), v1.y };
			Vector3 tc = { v2.x, _terrain->GetHeight(Vector3(v2.x, 0.0f, v2.y)), v2.y };
			triangles.push_back({ ta, tb, tc });

			indices.erase(indices.begin() + i);
			earFound = true;
			break;
		}
		if (!earFound)
		{
			cout << "Triangulation failed!" << endl;
			break;
		}
	}
	_regionIdToTriList[regionID] = triangles;
}

void NavMesh::CreateMesh()
{
	UINT index = 0;
	for (auto& triList : _regionIdToTriList)
	{
		for (auto& tri : triList.second)
		{
			Vertex v0, v1, v2;
			v0.position = tri.v0;
			v1.position = tri.v1;
			v2.position = tri.v2;
			
			_vertices.push_back(v0);
			_vertices.push_back(v1);
			_vertices.push_back(v2);

			_indices.push_back(index);
			_indices.push_back(index + 1);
			_indices.push_back(index + 2);

			index += 3;
		}
	}

	_mesh = new Mesh(_vertices.data(), sizeof(Vertex), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

bool NavMesh::CheckPointInTriangle(Vector3 pos, Triangle tri)
{
	Vector3 a = tri.v0;
	Vector3 b = tri.v1;
	Vector3 c = tri.v2;

	float denom = (b.z - c.z) * (a.x - c.x) + (c.x - b.x) * (a.z - c.z);
	float alpha = ((b.z - c.z) * (pos.x - c.x) + (c.x - b.x) * (pos.z - c.z)) / denom;
	float beta = ((c.z - a.z) * (pos.x - c.x) + (a.x - c.x) * (pos.z - c.z)) / denom;
	float gamma = 1.0f - alpha - beta;

	return alpha >= 0 && beta >= 0 && gamma >= 0;
}
