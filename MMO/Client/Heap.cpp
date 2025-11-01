#include "pch.h"
#include "Heap.h"
#include "Node.h"

void Heap::Insert(Node* node)
{
    _heap.emplace_back(node);
    UpdateUpper((int)_heap.size() - 1);
}

void Heap::UpdateUpper(int index)
{
    int curIndex = index;
    int parent = (curIndex - 1) / 2;

    while (curIndex != 0)
    {
        if (_heap[parent]->_f < _heap[curIndex]->_f)
            break;

        swap(_heap[curIndex], _heap[parent]);

        curIndex = parent;
        parent = (curIndex - 1) / 2;
    }
}

Node* Heap::DeleteRoot()
{
    Node* root = _heap.front();

    swap(_heap[0], _heap.back());

    _heap.pop_back();
    UpdateLower(0);

    return root;
}

void Heap::UpdateLower(int index)
{
    int curIndex = index;
    int lChild = index * 2 + 1;
    int rChild = lChild + 1;
    int minNode = curIndex;

    while (true)
    {
        if (lChild < _heap.size() && _heap[lChild]->_f < _heap[minNode]->_f)
            minNode = lChild;

        if (rChild < _heap.size() && _heap[rChild]->_f < _heap[minNode]->_f)
            minNode = rChild;

        if (minNode == curIndex)
            break;

        swap(_heap[curIndex], _heap[minNode]);

        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = lChild + 1;
    }
}

void Heap::Clear()
{
    _heap.clear();
}
