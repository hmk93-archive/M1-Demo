#include "pch.h"
#include "ModelObject.h"
#include "Model.h"
#include "Environment.h"
#include "Camera.h"
#include "Utility.h"
#include "BoxCollider.h"
using namespace Utility;

ModelObject::ModelObject(string file)
	: Transform(GetFileName(file))
{
	_model = new Model(file);
	_model->SetShader(L"Default");

	Vector3 minBox, maxBox;
	_model->SetBox(&minBox, &maxBox);

	pickingCollider = new BoxCollider(minBox, maxBox);
	pickingCollider->SetParent(&_world);
}

ModelObject::~ModelObject()
{
	delete pickingCollider;
	delete _model;
}

void ModelObject::Update()
{
	
}

void ModelObject::Render()
{
	SetWorldBuffer();
	_model->Render();
	pickingCollider->Render();
}

void ModelObject::PostRender()
{
	if (!isActive)
		return;
	TransformUsingGuizmo(_world, scale, rotation, position);
}