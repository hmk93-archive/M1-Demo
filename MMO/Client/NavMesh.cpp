#include "pch.h"
#include "NavMesh.h"
#include "Terrain.h"
#include "Mesh.h"
#include "Material.h"
#include "Device.h"
#include "RasterizerState.h"

NavMesh::NavMesh(UINT width, UINT height, bool debug)
	: _width(width)
	, _height(height)
{
	isActive = debug;

	InitNavMesh();

	_material = new Material(L"Collider");
	_material->GetBuffer()->data.diffuse = Vector4(0, 1, 0, 1);

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
	if (!isActive)
		return;

	SetWorldBuffer();

	_mesh->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_material->Set();

	_fillModes[1]->SetState();
	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_navIndices.size(), 0, 0);
	_fillModes[0]->SetState();
}

void NavMesh::Bake()
{
	if (!_solid)
		__debugbreak();

	std::vector<unsigned char> triAreas(_navIndices.size() / 3, RC_WALKABLE_AREA);
	rcMarkWalkableTriangles(&_ctx, _cfg.walkableSlopeAngle, (float*)_navVertices.data(), (int)_navVertices.size(), (int*)_navIndices.data(), (int)_navIndices.size() / 3, triAreas.data());
	
	int walkableCount = 0;
	for (auto area : triAreas)
		if (area == RC_WALKABLE_AREA)
			walkableCount++;

	// 1. 삼각형 데이터를 HeightField에 rasterize
	if (!rcRasterizeTriangles(&_ctx, (float*)_navVertices.data(), (int)_navVertices.size(), (int*)_navIndices.data(), triAreas.data(), (int)triAreas.size(), *_solid, 0))
	{
		__debugbreak();
	}

	// 낮은 장애물 제거
	rcFilterLowHangingWalkableObstacles(&_ctx, _cfg.walkableClimb, *_solid);

	// 낭떠러지 제거, 높은 ledge 제거
	rcFilterLedgeSpans(&_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid);

	// 낮은 높이 스팬 제거
	rcFilterWalkableLowHeightSpans(&_ctx, _cfg.walkableHeight, *_solid);

	// 2. Compact Height Field 생성
	_chf = rcAllocCompactHeightfield();
	if (!rcBuildCompactHeightfield(&_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid, *_chf))
	{
		__debugbreak();
	}

	// 3. 렌더링을 위한 ContourSet 과 PolyMesh 생성
	rcContourSet* cset = rcAllocContourSet();
	if (!rcBuildContours(&_ctx, *_chf, 0.5f, 0, *cset)) // maxError = 0.5, buildFlags = 0
	{
		__debugbreak();
	}

	_pmesh = rcAllocPolyMesh();
	if (!rcBuildPolyMesh(&_ctx, *cset, 6, *_pmesh))  // maxVertsPerPoly = 6
	{
		__debugbreak();
	}

	if(isActive)
		CreateMesh();
}

float NavMesh::GetHeightAt(Vector3 pos)
{
	if (_chf == nullptr)
		return 0.0f;

	int x = (int)((pos.x - _chf->bmin[0]) / _chf->cs);
	int z = (int)((pos.z - _chf->bmin[2]) / _chf->cs);
	if (x < 0 || x >= _chf->width || z < 0 || z >= _chf->height)
		return 0.0f;

	const rcCompactCell& cell = _chf->cells[x + z * _chf->width];
	if (cell.count == 0)
		return 0.0f;

	float bestY = -FLT_MAX;
	wstring log = L"CellCount: " + to_wstring(cell.count) + L"\n";
	OutputDebugString(log.c_str());
	for (int i = 0; i < (int)cell.count; ++i)
	{
		const rcCompactSpan& span = _chf->spans[cell.index + i];
		float smin = span.y * _chf->ch;

		// pos.y가 0 이거나 너무 낮으면, 단순히 가장 높은 스팬 반환
		if (pos.y <= 0.001f)
		{
			if (bestY == -FLT_MAX || smin < bestY)
				bestY = smin;
		}
		else
		{
			if (smin <= pos.y && smin > bestY)
				bestY = smin;
		}

		if (bestY == -FLT_MAX || smin < bestY)
			bestY = smin;
	}

	return (bestY != -FLT_MAX) ? bestY : 0.0f;
}

bool NavMesh::IsWalkable(Vector3 pos)
{
	float epsilon = 0.2f;

	int x = (int)((pos.x - _chf->bmin[0]) / _chf->cs);
	int z = (int)((pos.z - _chf->bmin[2]) / _chf->cs);
	if (x < 0 || x >= _chf->width || z < 0 || z >= _chf->height)
		return false;

	const rcCompactCell& cell = _chf->cells[x + z * _chf->width];
	for (int i = 0; i < (int)cell.count; ++i)
	{
		const rcCompactSpan& span = _chf->spans[cell.index + i];
		float smin = span.y * _chf->ch;
		float smax = smin + _chf->ch;

		if ((pos.y + epsilon >= smin) && pos.y <= smin + _cfg.walkableHeight * _chf->ch * 5.0f)
			return true;
	}
	return false;
}

void NavMesh::AppedMesh(const vector<Vector3>& vertices, const vector<UINT>& indices)
{
	size_t vertOffset = _navVertices.size();
	_navVertices.insert(_navVertices.end(), vertices.begin(), vertices.end());
	for (UINT index : indices)
		_navIndices.push_back(index + (UINT)vertOffset);
}

void NavMesh::InitNavMesh()
{
	_cfg.cs = 0.25f; // cell size
	_cfg.ch = 0.05f; // cell height
	_cfg.walkableSlopeAngle = 55.0f;
	_cfg.walkableHeight = (int)ceilf(2.0f / _cfg.ch);
	_cfg.walkableClimb = (int)floorf(0.9f / _cfg.ch);
	_cfg.walkableRadius = (int)ceilf(0.6f / _cfg.cs);

	// Heightfield 생성
	float bmin[3] = { 0, 0, 0 };
	float bmax[3] = { (float)_width, 25, (float)_height };
	float width = (int)((bmax[0] - bmin[0]) / _cfg.cs + 0.5f);
	float height = (int)((bmax[2] - bmin[2]) / _cfg.cs + 0.5f);
	_solid = rcAllocHeightfield();
	if (!rcCreateHeightfield(&_ctx, *_solid, width, height, bmin, bmax, _cfg.cs, _cfg.ch))
	{
		__debugbreak();
	}
}

void NavMesh::CreateMesh()
{
	// Vertex
	for (int i = 0; i < _pmesh->nverts; ++i)
	{
		float x = _pmesh->verts[i * 3 + 0];
		float y = _pmesh->verts[i * 3 + 1];
		float z = _pmesh->verts[i * 3 + 2];
		_renderVertices.push_back({ x, y, z });
	}

	// Index
	int nvp = _pmesh->nvp;
	for (int i = 0; i < _pmesh->npolys; ++i)
	{
		unsigned short* poly = &_pmesh->polys[i * nvp]; // i번째 poly 시작
		int vertsInPoly = 0;

		// 유효한 vertex 개수 확인 (RC_MESH_NULL_VERT = 0xffff)
		for (int j = 0; j < nvp; ++j)
		{
			if (poly[j] != RC_MESH_NULL_IDX) vertsInPoly++;
		}

		// fan triangulation
		for (int j = 2; j < vertsInPoly; ++j)
		{
			_renderIndices.push_back(poly[0]);
			_renderIndices.push_back(poly[j - 1]);
			_renderIndices.push_back(poly[j]);
		}
	}

	_mesh = new Mesh(_renderVertices.data(), sizeof(Vertex), (UINT)_renderVertices.size(), _renderIndices.data(), (UINT)_renderIndices.size());
}
