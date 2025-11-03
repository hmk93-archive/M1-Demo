#include "pch.h"
#include "Terrain.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "StructuredBuffer.h"
#include "ComputeShader.h"
#include "GlobalBuffers.h"
#include "Device.h"
#include "Environment.h"
#include "Camera.h"
#include "Input.h"

Terrain::Terrain(Vector3 pos)
{
	_material = new Material(L"Terrain");
	_material->SetDiffuseMap(L"../Assets/Textures/WallDiffuse.png");

	_secondMap = Texture::Add(L"../Assets/Textures/Floor.png");
	_thirdMap = Texture::Add(L"../Assets/Textures/Stones.png");

	_heightMap = Texture::Add(L"../Assets/Textures/HeightMap.png");

	CreateMesh();
	CreateCompute();

	_typeBuffer = new TypeBuffer();
}

Terrain::~Terrain()
{
	delete _typeBuffer;
	delete _material;
	delete _mesh;
	delete _rayBuffer;
	delete _structuredBuffer;
	delete[] _input;
	delete[] _output;
}

void Terrain::Update()
{
	UpdateWorld();
}

void Terrain::Render()
{
	_mesh->IASet();
	_worldBuffer->SetVSBuffer(0);
	_typeBuffer->SetVSBuffer(5);
	_material->Set();

	_secondMap->PSSet(11);
	_thirdMap->PSSet(12);

	Device::Get().GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}

void Terrain::PostRender()
{

}

void Terrain::CreateMesh()
{
	if (!_heightMap)
		return;

	_width = _heightMap->Width() - 1;
	_height = _heightMap->Height() - 1;
	vector<Vector4> pixels = _heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z <= _height; z++)
	{
		for (UINT x = 0; x <= _width; x++)
		{
			VertexType vertex;
			vertex.position = Vector3((float)x, 0.0f, (float)z);
			vertex.uv = Vector2((float)x / (float)_width, 1.0f -(z / (float)_height));

			UINT index = z * (_width + 1) + x;
			vertex.position.y += pixels[index].x * 20.0f;

			_vertices.emplace_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < _height; z++)
	{
		for (UINT x = 0; x < _width; x++)
		{
			_indices.emplace_back(_width * (z + 0) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 1));

			_indices.emplace_back(_width * (z + 0) + (x + 0));
			_indices.emplace_back(_width * (z + 1) + (x + 1));
			_indices.emplace_back(_width * (z + 0) + (x + 1));
		}
	}

	_computeSize = (UINT)_indices.size() / 3;

	CreateInput();

	if (_mesh)
		delete _mesh;

	CreateAlpha();

	_mesh = new Mesh(_vertices.data(), sizeof(VertexType), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

void Terrain::CreateCompute()
{
	_computeShader = Shader::AddCS(L"ComputePicking");

	_structuredBuffer = new StructuredBuffer(_input, sizeof(InputDesc), _computeSize, sizeof(OutputDesc), _computeSize);

	_rayBuffer = new RayBuffer();
	
	_output = new OutputDesc[_computeSize];
}

void Terrain::CreateInput()
{
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

void Terrain::CreateAlpha()
{
	wstring filenames[2] =
	{
		L"../Assets/Textures/First.png",
		L"../Assets/Textures/Second.png"
	};

	for (UINT i = 0; i < 2; i++)
	{
		_alphaMap[i] = Texture::Load(filenames[i]);
		vector<Vector4> pixels = _alphaMap[i]->ReadPixels();
		for (UINT z = 0; z < _height; z++)
		{
			for (UINT x = 0; x < _width; x++)
			{
				UINT index = _width * z + x;
				switch (i)
				{
				case 0:
					_vertices[index].alpha[i] = pixels[index].x;
					break;
				case 1:
					_vertices[index].alpha[i] = pixels[index].y;
					break;
				default:
					break;
				}
			}
		}
	}
}

bool Terrain::ComputePicking(OUT Vector3& position)
{
	Ray ray = Environment::Get().GetMainCamera()->ScreenPointToRay(Input::Get().GetMouse());
	_rayBuffer->data.position = ray.position;
	_rayBuffer->data.direction = ray.direction;
	_rayBuffer->data.size = (float)_computeSize;
	_computeShader->Set();

	_rayBuffer->SetCSBuffer(0);

	Device::Get().GetDeviceContext()->CSSetShaderResources(0, 1, &_structuredBuffer->GetSRV());
	Device::Get().GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &_structuredBuffer->GetUAV(), nullptr);

	UINT x = (UINT)ceil((float)_computeSize / 1024.0f);

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

float Terrain::GetHeight(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > _width) return 0.0f;
	if (z < 0 || z > _height) return 0.0f;

	UINT index[4];
	index[0] = (_width + 1) * (z + 0) + (x + 0);
	index[1] = (_width + 1) * (z + 1) + (x + 0);
	index[2] = (_width + 1) * (z + 0) + (x + 1);
	index[3] = (_width + 1) * (z + 1) + (x + 1);

	for (int i = 0; i < 4; ++i)
		if (index[i] < 0 || index[i] > _vertices.size())
			return 0;

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = _vertices[index[i]].position;

	float u = position.x - p[0].x;
	float v = position.z - p[0].z;

	Vector3 result;
	if (u + v <= 1.0f)
	{
		result = p[0] + (p[2] - p[0]) * u + (p[1] - p[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.y;
}
