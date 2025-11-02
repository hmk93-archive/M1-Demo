#include "pch.h"
#include "ModelObject.h"
#include "Model.h"
#include "Environment.h"
#include "Camera.h"

ModelObject::ModelObject(string file)
{
	_model = new Model(file);
	_model->SetShader(L"Default");

	Vector3 minBox, maxBox;
	_model->SetBox(&minBox, &maxBox);
}

ModelObject::~ModelObject()
{
	delete _model;
}

void ModelObject::Update()
{
	
}

void ModelObject::Render()
{
	SetWorldBuffer();
	_model->Render();
}

void ModelObject::PostRender()
{
	ImGuizmo::Manipulate(
		(float*)&Environment::Get().GetMainCamera()->GetView(), 
		(float*)&Environment::Get().GetProjection(), 
		ImGuizmo::TRANSLATE, 
		ImGuizmo::LOCAL, 
		(float*)&_world);
}