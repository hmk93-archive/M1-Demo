#pragma once

class Node;
class Heap;
class Collider;

class AStar
{
public:
	AStar(UINT width = 100, UINT height = 100);
	~AStar();

	void Update();
	void Render();
	void PostRender();

	void SetNode(Vector2 size);
	void SetObstacle(vector<class ModelObject*> value);

	int FindCloseNode(Vector3 pos);
	vector<Vector3> FindPath(int start, int end);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	bool CollisionObstacle(Ray ray, float destDistance);
	
	void SetPath(vector<Vector3>& path, Vector3 position, Vector3 destPos);

	void Reset();

	void MakeObsAt(UINT idx);

private:
	float GetDistance(int curIndex, int end);
	void Extend(int center, int end);
	int GetMinNode();
	void SetCullIdx();

public:
	bool makeObsMode = false;
	bool showNode = false;
	POINT cullIdx = {};
	POINT cullRange = { 10, 10 };
	vector<Node*> nodes = {};
	UINT width = 0, height = 0;

private:
	Vector2 _fieldSize = Vector2::Zero;

	Heap* _heap = nullptr;

	Vector2 _interval = Vector2::Zero;

	vector<Collider*> _obstacles = {};
};