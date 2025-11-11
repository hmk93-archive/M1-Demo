#include "pch.h"
#include "UIImage.h"
#include "Material.h"
#include "Mesh.h"
#include "GlobalBuffers.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Device.h"

UIImage::UIImage(wstring shaderFile, UIImage::Pivot inPivot, bool blendAlpha, bool depthEnable)
	: pivot(inPivot)
{
	_material = new Material(shaderFile);
	_material->GetBuffer()->data.diffuse = Vector3(1.0f);
	CreateMesh();
	CreateMatrix();

	_blendState[0] = new BlendState();
	_blendState[1] = new BlendState();
	_blendState[1]->Alpha(blendAlpha);

	_depthMode[0] = new DepthStencilState();
	_depthMode[1] = new DepthStencilState();
	_depthMode[1]->DepthEnable(depthEnable);
}

UIImage::~UIImage()
{
	delete _depthMode[1];
	delete _depthMode[0];

	delete _blendState[1];
	delete _blendState[0];

	delete _viewBuffer;
	delete _orthoBuffer;

	delete _mesh;
	delete _material;
}

void UIImage::Update()
{

}

void UIImage::Render()
{
	_mesh->IASet();

	SetWorldBuffer();
	_viewBuffer->SetVSBuffer(1);
	_orthoBuffer->SetVSBuffer(2);

	Device::Get().GetDeviceContext()->PSSetShaderResources(0, 1, _srv.GetAddressOf());

	_material->Set();

	_blendState[1]->SetState();
	_depthMode[1]->SetState();
	Device::Get().GetDeviceContext()->DrawIndexed(6, 0, 0);
	_blendState[0]->SetState();
	_depthMode[0]->SetState();
}

void UIImage::CreateMesh()
{
	_vertices.resize(4);

	switch (pivot)
	{
	case Center:
		_vertices[0].position = { -0.5f, -0.5f, 0.0f };
		_vertices[1].position = { -0.5f, +0.5f, 0.0f };
		_vertices[2].position = { +0.5f, -0.5f, 0.0f };
		_vertices[3].position = { +0.5f, +0.5f, 0.0f };
		break;
	case LC:
		_vertices[0].position = { +0.0f, -0.5f, 0.0f };
		_vertices[1].position = { +0.0f, +0.5f, 0.0f };
		_vertices[2].position = { +1.0f, -0.5f, 0.0f };
		_vertices[3].position = { +1.0f, +0.5f, 0.0f };
		break;
	}

	_vertices[0].uv = { 0, 1 };
	_vertices[1].uv = { 0, 0 };
	_vertices[2].uv = { 1, 1 };
	_vertices[3].uv = { 1, 0 };

	UINT indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	_mesh = new Mesh(_vertices.data(), sizeof(VertexUV), (UINT)_vertices.size(), indices, 6);
}

void UIImage::CreateMatrix()
{
	if (_viewBuffer)
		delete _viewBuffer;
	if (_orthoBuffer)
		delete _orthoBuffer;

	_view = XMMatrixIdentity();
	_ortho = XMMatrixOrthographicOffCenterLH(0, g_screenWidth, 0, g_screenHeight, -1, 1);

	_viewBuffer = new MatrixBuffer();
	_viewBuffer->Set(_view);

	_orthoBuffer = new MatrixBuffer();
	_orthoBuffer->Set(_ortho);
}
