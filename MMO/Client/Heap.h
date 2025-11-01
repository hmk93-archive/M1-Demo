#pragma once

class Node;

class Heap
{
public:
	Heap() = default;
	~Heap() = default;

	void Insert(Node* node);
	void UpdateUpper(int index);

	Node* DeleteRoot();
	void UpdateLower(int index);

	void Clear();

private:
	vector<Node*> _heap = {};
};