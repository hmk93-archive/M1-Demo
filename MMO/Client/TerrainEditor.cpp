#include "pch.h"
#include "TerrainEditor.h"
#include "Material.h"
#include "Mesh.h"
#include "Device.h"
#include "Shader.h"
#include "StructuredBuffer.h"
#include "Environment.h"
#include "Camera.h"
#include "Control.h"
#include "ComputeShader.h"
#include "Timer.h"
#include "Utility.h"
#include "Texture.h"
using namespace Utility;

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: _width(width)
	, _height(height)
{
	_material = new Material(L"TerrainEditor");
	_material->SetDiffuseMap(L"../Assets/Textures/WallDiffuse.png");

	CreateMesh();
	CreateCompute();

	_brushBuffer = new BrushBuffer();

	InitFileSystem();
}

TerrainEditor::~TerrainEditor()
{
	delete _brushBuffer;
	delete _rayBuffer;
	delete _structuredBuffer;
	delete[] _input;
	delete[] _output;
	delete _mesh;
	delete _material;
}

void TerrainEditor::Update()
{
	if (Control::Get().Press(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		AdjustY(_brushBuffer->data.location);
	}

	if (Control::Get().Up(VK_LBUTTON))
	{
		_mesh->UpdateVertex(_vertices.data(), _vertices.size());
	}

	UpdateWorld();
}

void TerrainEditor::Render()
{
	_mesh->IASet();
	_material->Set();
	SetWorldBuffer();

	_worldBuffer->SetVSBuffer(0);

	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void TerrainEditor::PostRender()
{
	Vector3 temp;
	ComputePicking(temp);
	_brushBuffer->data.location = temp;

	ImGui::Text("[Terrain Editor]");
	ImGui::Text("Mouse Position: (%f %f %f)", _brushBuffer->data.location.x, _brushBuffer->data.location.y, _brushBuffer->data.location.z);

	FileSystem();
}

void TerrainEditor::CreateMesh()
{
	vector<Vector4> pixels;

	if (_heightMap)
	{
		_width = _heightMap->Width();
		_height = _heightMap->Height();
		pixels = _heightMap->ReadPixels();
	}

	_vertices.clear();
	_indices.clear();

	//Vertices
	for (UINT z = 0; z < _height; z++)
	{
		for (UINT x = 0; x < _width; x++)
		{
			VertexType vertex;
			vertex.position = Vector3((float)x, 0.0f, (float)z);
			vertex.uv = Vector2((float)x / (float)_width, 1.0f - (z / (float)_height));

			// Set Vertex PosY from HeightMap
			UINT index = (z * _width) + x;
			if (pixels.size() > index)
				vertex.position.y += pixels[index].x * MAX_HEIGHT;

			_vertices.emplace_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < _height - 1; z++)
	{
		for (UINT x = 0; x < _width - 1; x++)
		{
			_indices.emplace_back(_width * (z + 0) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 1));

			_indices.emplace_back(_width * (z + 0) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 1));
			_indices.emplace_back(_width * (z + 0) + (x + 1));
		}
	}

	_computeSize = _indices.size() / 3;

	SetInput();

	if (_mesh)
		delete _mesh;

	_mesh = new Mesh(_vertices.data(), sizeof(VertexType), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

void TerrainEditor::CreateCompute()
{
	if (!_computeShader)
		_computeShader = Shader::AddCS(L"ComputePicking");

	if (_structuredBuffer)
		delete _structuredBuffer;
	_structuredBuffer = new StructuredBuffer(_input, sizeof(InputDesc), _computeSize, sizeof(OutputDesc), _computeSize);

	if (_rayBuffer)
		delete _rayBuffer;
	_rayBuffer = new RayBuffer();

	if (!_output)
		delete[] _output;
	_output = new OutputDesc[_computeSize];
}

void TerrainEditor::SetInput()
{
	if (_input != nullptr)
		delete[] _input;
	_input = new InputDesc[_computeSize];

	for (UINT i = 0; i < _computeSize; i++)
	{
		UINT index0 = _indices[(i * 3) + 0];
		UINT index1 = _indices[(i * 3) + 1];
		UINT index2 = _indices[(i * 3) + 2];

		_input[i].v0 = _vertices[index0].position;
		_input[i].v1 = _vertices[index1].position;
		_input[i].v2 = _vertices[index2].position;

		_input[i].index = i;
	}
}

bool TerrainEditor::ComputePicking(OUT Vector3& position)
{
	Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Control::Get().GetMouse());

	_rayBuffer->data.position = ray.position;
	_rayBuffer->data.direction = ray.direction;
	_rayBuffer->data.size = _computeSize;
	_computeShader->Set();

	_rayBuffer->SetCSBuffer(0);

	Device::Get().GetDeviceContext()->CSSetShaderResources(0, 1, &_structuredBuffer->GetSRV());
	Device::Get().GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &_structuredBuffer->GetUAV(), nullptr);

	UINT x = ceil((float)_computeSize / 1024.0f);

	Device::Get().GetDeviceContext()->Dispatch(x, 1, 1);

	_structuredBuffer->Copy(_output, sizeof(OutputDesc) * _computeSize);

	float minDistance = FLT_MAX;
	int minIndex = -1;
	for (UINT i = 0; i < _computeSize; i++)
	{
		OutputDesc temp = _output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		position = ray.position + ray.direction * minDistance;
		return true;
	}

	return false;
}

void TerrainEditor::AdjustY(Vector3 position)
{
	switch (_brushBuffer->data.type)
	{
	case 0:
		for (VertexType& vertex : _vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.x, 0, position.z);

			float distance = (p2 - p1).Length();

			float temp = _adjustValue * max(0.0f, cos(XM_PIDIV2 * distance / _brushBuffer->data.range));

			if (distance <= _brushBuffer->data.range)
			{
				if (_isRaise)
					vertex.position.y += temp * Timer::Get().GetElapsedTime();
				else
					vertex.position.y -= temp * Timer::Get().GetElapsedTime();

				if (vertex.position.y < 0)
					vertex.position.y = 0;
				else if (vertex.position.y > MAX_HEIGHT)
					vertex.position.y = MAX_HEIGHT;
			}
		}
		break;
	}

	_mesh->UpdateVertex(_vertices.data(), _vertices.size());
}

void TerrainEditor::InitFileSystem()
{
	igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", 0, ".");
	_projectPath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
	igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
}

void TerrainEditor::FileSystem()
{
	SaveFile();
	LoadFile();
}

void TerrainEditor::SaveFile()
{
	if (ImGui::Button("Save"))
	{
		igfd::ImGuiFileDialog::Instance()->OpenDialog("SaveKey", "Save File", ".png", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("SaveKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
		{
			_path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			// _path = _path.substr(_projectPath.size() + 1, _path.length());
			Replace(&_path, "\\", "/");
			SaveHeight(ToWString(_path));
		}

		igfd::ImGuiFileDialog::Instance()->CloseDialog("SaveKey");
	}
}

void TerrainEditor::LoadFile()
{
	if (ImGui::Button("Load"))
	{
		igfd::ImGuiFileDialog::Instance()->OpenDialog("LoadKey", "Load File", ".png", ".");
	}

	if (igfd::ImGuiFileDialog::Instance()->FileDialog("LoadKey"))
	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
		{
			_path = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
			// _path = _path.substr(_projectPath.size() + 1, _path.length());
			Replace(&_path, "\\", "/");
			LoadHeight(ToWString(_path));
		}
		// close
		igfd::ImGuiFileDialog::Instance()->CloseDialog("LoadKey");
	}
}

void TerrainEditor::SaveHeight(wstring heightFile)
{
	UINT pixelSize = _width * _height * 4;
	uint8_t* pixels = new uint8_t[pixelSize];

	for (UINT i = 0; i < pixelSize / 4; i++)
	{
		float y = _vertices[i].position.y;

		uint8_t height = y * 255 / MAX_HEIGHT;

		pixels[(i * 4) + 0] = height;
		pixels[(i * 4) + 1] = height;
		pixels[(i * 4) + 2] = height;
		pixels[(i * 4) + 3] = 255;
	}

	Image image;
	image.width = _width;
	image.height = _height;
	image.pixels = pixels;
	image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	image.rowPitch = image.width * 4;
	image.slicePitch = image.width * image.height * 4;

	SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), heightFile.c_str());

	delete[] pixels;
}

void TerrainEditor::LoadHeight(wstring heightFile)
{
	_heightMap = Texture::Load(heightFile);

	delete _mesh;
	_mesh = nullptr;

	CreateMesh();
	CreateCompute();
}
