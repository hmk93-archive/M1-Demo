#pragma once

class Collider;

class Node
{
	friend class AStar;
	friend class Heap;

	struct EdgeInfo
	{
		int index;
		float edgeCost;
	};

public:
	enum State
	{
		None,
		Open,
		Closed,
		Using,
		Obstacle
	} state;

private:
	Node(Vector3 pos, int index, Vector2 interval);
	~Node();

	void Render();

	void AddEdge(Node* node);

	Collider* MakeObstacle();

public:
	Collider* GetCollider() { return _collider; }

private:
	Collider* _collider = nullptr;	
	Collider* _obstacle = nullptr;

	Vector3 _pos = Vector3::Zero;
	int _index = 0;
	int _via = -1;

	float _f = 0.0f;
	float _g = 0.0f;
	float _h = 0.0f;

	Vector2 _interval = Vector2::Zero;

	vector<EdgeInfo*> _edges = {};
};