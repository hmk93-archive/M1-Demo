#include "pch.h"
#include "AStar.h"
#include "Heap.h"
#include "Node.h"
#include "Collider.h"
#include "ModelObject.h"
#include "Utility.h"
using namespace Utility;

AStar::AStar(UINT width, UINT height)
	: width(width), height(height)
{
	_heap = new Heap();
}

AStar::~AStar()
{
	for (Node* node : nodes)
		delete node;

	delete _heap;
}

void AStar::Update()
{
	SetCullIdx();
	
	if (makeObsMode == false)
		return;
	//
	//if (KEY_DOWN(VK_LBUTTON))
	//{
	//	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

	//	for (int z = -2; z < 2; ++z)
	//		for (int x = -2; x < 2; ++x)
	//		{
	//			int idx = (cullIdx.y + z) * width + (cullIdx.x + x); //z * width + x;
	//			idx = Saturate(idx, 0, nodes.size() - 1);
	//			//
	//			if (nodes[idx]->collider->RayCollision(ray))
	//			{
	//				obstacles.emplace_back(nodes[idx]->MakeObstacle());
	//				break;
	//			}
	//		}
	//}
}

void AStar::Render()
{
	if (showNode == false) 
		return;
	
	for (int z = -cullRange.y; z < cullRange.y; ++z)
		for (int x = -cullRange.x; x < cullRange.x; ++x)
		{
			int idx = (cullIdx.y + z) * width + (cullIdx.x + x); //z * width + x;
			idx = clamp(idx, 0, (int)nodes.size() - 1);
			nodes[idx]->Render();
		}

	//for (Node* node : nodes)
	//	node->Render();	
}

void AStar::PostRender()
{
	ImGui::Text("=====[AStar]=====");
	ImGui::Checkbox("showNode", &showNode);
	ImGui::Checkbox("MakeObsMode", &makeObsMode);
	ImGui::Text("CullIdx [%d, %d]", cullIdx.x, cullIdx.y);
}

void AStar::SetNode(Vector2 size)
{
	_fieldSize = size;

	_interval.x = _fieldSize.x / width;
	_interval.y = _fieldSize.y / height;

	if (nodes.empty() == false)
	{
		for (auto node : nodes)
			delete node;
		nodes.clear();
	}
		//
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			Vector3 pos = Vector3(x * _interval.x, 0, z * _interval.y);

			int index = z * width + x;
			nodes.emplace_back(new Node(pos, index, _interval));
		}
	}

	for (UINT i = 0; i < nodes.size(); i++)
	{
		if (i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width)
		{
			nodes[i + 0]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width && i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width && i % width != 0)
		{
			nodes[i + 0]->AddEdge(nodes[i + width - 1]);
			nodes[i + width - 1]->AddEdge(nodes[i + 0]);
		}
	}
}

void AStar::SetObstacle(vector<class ModelObject*> value)
{
	//for (ModelObject* object : value)
	//{
	//	Collider* obstacle = object->GetCollider();

	//	for (Node* node : nodes)
	//	{
	//		node->collider->UpdateWorld();
	//		if (obstacle->Collision(node->collider))
	//		{
	//			node->state = Node::OBSTACLE;
	//		}
	//	}

	//	obstacles.emplace_back(obstacle);
	//}
}

int AStar::FindCloseNode(Vector3 pos)
{
	float minDistance = FLT_MAX;
	int index = -1;

	for (UINT i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->state == Node::Obstacle)
			continue;

		float distance = (pos - nodes[i]->_pos).Length();

		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}
	}

	return index;
}

vector<Vector3> AStar::FindPath(int start, int end)
{
	Reset();

	float G = 0.0f;
	float H = GetDistance(start, end);

	nodes[start]->_g = G;
	nodes[start]->_h = H;
	nodes[start]->_f = G + H;
	nodes[start]->_via = start;
	nodes[start]->state = Node::Open;

	_heap->Insert(nodes[start]);

	while (nodes[end]->state != Node::Closed)
	{
		int curIndex = GetMinNode();
		Extend(curIndex, end);
		nodes[curIndex]->state = Node::Closed;
	}

	vector<Vector3> path;

	int curIndex = end;
	while (curIndex != start)
	{
		nodes[curIndex]->state = Node::Using;
		path.emplace_back(nodes[curIndex]->_pos);
		curIndex = nodes[curIndex]->_via;
	}

	nodes[curIndex]->state = Node::Using;
	path.emplace_back(nodes[curIndex]->_pos);

	_heap->Clear();

	return path;
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
	int cutNodeNum = 0;
	Ray ray;
	ray.position = start;

	for (UINT i = 0; i < path.size(); i++)
	{
		ray.direction = path[i] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!CollisionObstacle(ray, distance))
		{
			cutNodeNum = (int)path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.pop_back();

	cutNodeNum = 0;
	ray.position = end;

	for (UINT i = 0; i < path.size(); i++)
	{
		ray.direction = path[(int)path.size() - i - 1] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!CollisionObstacle(ray, distance))
		{
			cutNodeNum = (int)path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.erase(path.begin());
}

bool AStar::CollisionObstacle(Ray ray, float destDistance)
{
	for (Collider* obstacle : _obstacles)
	{
		Contact contact;

		if (obstacle->RayCollision(ray, &contact))
		{
			if (contact.distance < destDistance)
				return true;
		}
	}

	return false;
}

void AStar::SetPath(vector<Vector3>& path, Vector3 position, Vector3 destPos)
{
	path.clear();
	//
	Ray ray;
	ray.position = position;
	ray.direction = (destPos - position);
	ray.direction.Normalize();

	float distance = (destPos - position).Length();

	if (CollisionObstacle(ray, distance))
	{
		int startIndex = FindCloseNode(position);
		int endIndex = FindCloseNode(destPos);

		path = FindPath(startIndex, endIndex);
		path.insert(path.begin(), destPos);

		MakeDirectPath(position, destPos, path);
		path.insert(path.begin(), destPos);

		UINT pathSize = (UINT)path.size();

		while (path.size() > 2)
		{
			vector<Vector3> tempPath;
			for (UINT i = 1; i < path.size() - 1; i++)
				tempPath.emplace_back(path[i]);

			Vector3 start = path.back();
			Vector3 end = path.front();

			MakeDirectPath(start, end, tempPath);

			path.clear();
			path.emplace_back(end);

			for (Vector3 temp : tempPath)
				path.emplace_back(temp);

			path.emplace_back(start);

			if (pathSize == (UINT)path.size())
				break;
			else
				pathSize = (UINT)path.size();
		}
	}
	else
	{
		path.insert(path.begin(), destPos);
	}
}

void AStar::Reset()
{
	for (Node* node : nodes)
	{
		if (node->state != Node::Obstacle)
			node->state = Node::None;
	}
}

void AStar::MakeObsAt(UINT idx)
{
	_obstacles.emplace_back(nodes[idx]->MakeObstacle());
}

float AStar::GetDistance(int curIndex, int end)
{
	Vector3 startPos = nodes[curIndex]->_pos;
	Vector3 endPos = nodes[end]->_pos;

	return (startPos - endPos).Length();
}

void AStar::Extend(int center, int end)
{
	vector<Node::EdgeInfo*> edges = nodes[center]->_edges;

	for (UINT i = 0; i < edges.size(); i++)
	{
		int index = edges[i]->index;

		if (nodes[index]->state == Node::Closed ||
			nodes[index]->state == Node::Obstacle)
			continue;

		float G = nodes[center]->_g + edges[i]->edgeCost;
		float H = GetDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == Node::Open)
		{
			if (F < nodes[index]->_f)
			{
				nodes[index]->_g = G;
				nodes[index]->_f = F;
				nodes[index]->_via = center;
			}
		}
		else if (nodes[index]->state == Node::None)
		{
			nodes[index]->_g = G;
			nodes[index]->_h = H;
			nodes[index]->_f = F;
			nodes[index]->_via = center;
			nodes[index]->state = Node::Open;

			_heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	return _heap->DeleteRoot()->_index;
}

void AStar::SetCullIdx()
{
	Vector3 mouseWorldPos = ScreenToWorldHeightZero();
	mouseWorldPos.x = clamp(mouseWorldPos.x, 0.0f, _fieldSize.x);
	mouseWorldPos.z = clamp(mouseWorldPos.z, 0.0f, _fieldSize.y);

	cullIdx.x = (mouseWorldPos.x / _fieldSize.x) * width;
	cullIdx.y = (mouseWorldPos.z / _fieldSize.y) * height;
}
