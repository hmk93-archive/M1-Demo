#pragma once

#include "Transform.h"

class Model;
class Collider;

class ModelObject : public Transform
{
public:
	ModelObject(string file);
	virtual ~ModelObject();

	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	Model* GetModel() { return _model; }

public:
	Collider* pickingCollider = nullptr;

protected:
	Model* _model = nullptr;
	string _name = "";
};

