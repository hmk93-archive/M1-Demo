#include "pch.h"
#include "Circle.h"
#include "Material.h"
#include "Mesh.h"
#include "BlendState.h"
#include "Device.h"

Circle::Circle(wstring textureFile, float radius, float angle, UINT sliceCount, wstring shaderFile)
	: radius(radius)
	, angle(angle)
	, sliceCount(sliceCount)
{
	material = new Material(shaderFile);
	wstring filePath = L"../Assets/Textures/" + textureFile + L".png";
	material->SetDiffuseMap(filePath);
	materialBuffer = new MaterialBuffer();

	CreateMesh();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
}

Circle::~Circle()
{
	delete blendState[1];
	delete blendState[0];
	delete materialBuffer;
	delete mesh;
	delete material;
}

void Circle::Update()
{
	if (!isActive)
		return;

	UpdateWorld();
}

void Circle::Render()
{
	if (!isActive)
		return;

	mesh->IASet();
	SetWorldBuffer();
	material->Set();
	blendState[1]->SetState();
	Device::Get().GetDeviceContext()->DrawIndexed((UINT)indices.size(), 0, 0);
	blendState[0]->SetState();
	RenderAxis();
}

void Circle::SetColor(Vector4 color)
{
	material->GetBuffer()->data.diffuse = color;
}

void Circle::CreateMesh()
{
	float thetaStep = angle / sliceCount;

	// Vertex
	VertexType vertex;
	vertex.position = {};
	vertex.uv = { 0.5f, 0.5f };
	vertices.emplace_back(vertex);

	float x, z, theta;
	for (UINT i = 0; i < sliceCount; i++)
	{
		theta = i * thetaStep;
		x = cos(theta);
		z = sin(theta);
		
		vertex.position = Vector3(x, 0.1f, z) * radius;
		vertex.uv.x = vertices[0].uv.x + (+x * 0.5f);
		vertex.uv.y = vertices[0].uv.y + (-z * 0.5f);
		
		vertices.emplace_back(vertex);
	}

	// Index
	for (UINT i = 0; i < sliceCount; i++)
	{
		int i1 = i + 1;
		int i2 = i1 + 1;
		if ((UINT)i2 > sliceCount)
			i2 -= sliceCount;
		indices.emplace_back(0);
		indices.emplace_back(i2);
		indices.emplace_back(i1);
	}

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
}
