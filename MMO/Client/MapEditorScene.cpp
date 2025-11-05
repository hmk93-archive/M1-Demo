#include "pch.h"
#include "MapEditorScene.h"
#include "TerrainEditor.h"
#include "ModelObject.h"
#include "Environment.h"
#include "Camera.h"
#include "Input.h"
#include "Collider.h"
#include "tinyxml2.h"
#include "Utility.h"
using namespace Utility;

MapEditorScene::MapEditorScene()
{
	// Terrain Editor
	_terrainEditor = new TerrainEditor(300, 300);
	// Create ModelObject
	{
		{
			ModelObject* obj = new ModelObject("Stone/Stone");
			obj->rotation.z = XM_PI;
			_models.emplace_back(obj);
		}
		{
			ModelObject* obj = new ModelObject("House/House");
			obj->scale = Vector3(0.01f);
			_models.emplace_back(obj);
		}
	}

	for (auto& model : _models)
		model->isActive = false;

	InitFileSystem();
}

MapEditorScene::~MapEditorScene()
{
	delete _terrainEditor;
	for (ModelObject* model : _models)
		delete model;
}

void MapEditorScene::Update()
{
	_terrainEditor->Update();

	// Picking and update
	float minDistance = FLT_MAX;
	ModelObject* closestModel = nullptr;
	for (ModelObject* model : _models)
	{
		if (Input::Get().Down(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
		{
			Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Input::Get().GetMouse());
			Contact contact = {};
			if (model->collider->RayCollision(ray, &contact))
			{
				if (minDistance > contact.distance)
				{
					minDistance = contact.distance;
					closestModel = model;
				}
			}
		}
		model->Update();
	}

	if (closestModel)
	{
		// Reset active
		for (ModelObject* model : _models)
			model->isActive = false;

		closestModel->isActive = true;
	}
}

void MapEditorScene::Render()
{
	_terrainEditor->Render();
	for (ModelObject* model : _models)
		model->Render();
}

void MapEditorScene::PostRender()
{
	_terrainEditor->PostRender();
	for (ModelObject* model : _models)
		model->PostRender();

	ImGui::Begin("[Map]");
	FileSystem();
	ImGui::End();
}

void MapEditorScene::InitFileSystem()
{
	igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", 0, ".");
	_projectPath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
	igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
}

void MapEditorScene::FileSystem()
{
	SaveFile();
	LoadFile();
}

void MapEditorScene::SaveFile()
{
	if (ImGui::Button("MapSave"))
	{
		igfd::ImGuiFileDialog::Instance()->OpenDialog("MapSaveKey", "Save Map File", ".xml", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("MapSaveKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
		{
			_path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			Replace(&_path, "\\", "/");
			SaveMap(_path);
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("MapSaveKey");
	}
}

void MapEditorScene::LoadFile()
{
	if (ImGui::Button("MapLoad"))
	{
		igfd::ImGuiFileDialog::Instance()->OpenDialog("MapLoadKey", "Load Map File", ".xml", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("MapLoadKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
		{
			_path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			Replace(&_path, "\\", "/");
			LoadMap(_path);
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("MapLoadKey");
	}
}

void MapEditorScene::SaveMap(string path)
{
	CreateFolders(path);

	XmlDocument* document = new XmlDocument();

	XmlElement* root = document->NewElement("Map");
	document->LinkEndChild(root);

	string folder = GetDirectoryName(path);

	for (ModelObject* model : _models)
	{
		XmlElement* node = document->NewElement("ModelObject");
		root->LinkEndChild(node);

		XmlElement* element = document->NewElement("Name");
		element->SetText(model->tag.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Scale");
		element->SetAttribute("x", model->scale.x);
		element->SetAttribute("y", model->scale.y);
		element->SetAttribute("z", model->scale.z);
		node->LinkEndChild(element);

		element = document->NewElement("Rotation");
		element->SetAttribute("x", model->rotation.x);
		element->SetAttribute("y", model->rotation.y);
		element->SetAttribute("z", model->rotation.z);
		node->LinkEndChild(element);

		element = document->NewElement("Position");
		element->SetAttribute("x", model->position.x);
		element->SetAttribute("y", model->position.y);
		element->SetAttribute("z", model->position.z);
		node->LinkEndChild(element);
	}

	document->SaveFile(path.c_str());

	delete document;
}

void MapEditorScene::LoadMap(string path)
{
}

