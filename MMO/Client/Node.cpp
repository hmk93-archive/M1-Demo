#include "pch.h"
#include "Node.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

Node::Node(Vector3 pos, int index, Vector2 interval)
	: _pos(pos)
	, _index(index)
	, _interval(interval)
	, state(None)
{
	_collider = new SphereCollider();
	_collider->position = pos;
}

Node::~Node()
{
	delete _collider;
	if(_obstacle != nullptr)
		delete _obstacle;

	for (EdgeInfo* edge : _edges)
		delete edge;
}

void Node::Render()
{
	switch (state)
	{
	case Node::None:
		_collider->SetColor(Vector4(1, 1, 1, 1));
		break;
	case Node::Open:
		_collider->SetColor(Vector4(0, 0, 1, 1));
		break;
	case Node::Closed:
		_collider->SetColor(Vector4(1, 0, 0, 1));
		break;
	case Node::Using:
		_collider->SetColor(Vector4(0, 1, 0, 1));
		break;
	case Node::Obstacle:
		_collider->SetColor(Vector4(0, 0, 0, 1));
		break;	
	}

	_collider->Render();
	if(_obstacle != nullptr)
		_obstacle->Render();
}

void Node::AddEdge(Node* node)
{
	EdgeInfo* edge = new EdgeInfo();
	edge->index = node->_index;
	edge->edgeCost = (_pos - node->_pos).Length();

	_edges.emplace_back(edge);
}

Collider* Node::MakeObstacle()
{
	state = Obstacle;

	Vector3 min = { -_interval.x * 0.6f, -20, -_interval.y * 0.6f };
	Vector3 max = min * -1.0f;

	_obstacle = new BoxCollider(min, max);
	_obstacle->position = _collider->position;
	_obstacle->SetColor(Vector4(1, 0, 0, 1));

	return _obstacle;
}
