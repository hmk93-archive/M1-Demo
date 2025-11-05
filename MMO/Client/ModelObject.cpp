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

	collider = new BoxCollider(minBox, maxBox);
	collider->SetParent(&_world);
}

ModelObject::~ModelObject()
{
	delete collider;
	delete _model;
}

void ModelObject::Update()
{
	Transform::UpdateWorld();
	collider->UpdateWorld();
}

void ModelObject::Render()
{
	SetWorldBuffer();
	_model->Render();
	collider->Render();
}

void ModelObject::PostRender()
{
	if (!isActive)
		return;
	TransformUsingGuizmo(_world, scale, rotation, position);
}