#pragma once

#include "Transform.h"

class Model;

class ModelObject : public Transform
{
public:
	ModelObject(string file);
	virtual ~ModelObject();

	virtual void Update();
	virtual void Render();

	Model* GetModel() { return _model; }

protected:
	Model* _model = nullptr;
};

