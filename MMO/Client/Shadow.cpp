#include "pch.h"
#include "Shadow.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "UIImage.h"
#include "GlobalBuffers.h"
#include "Environment.h"
#include "Device.h"

Shadow::Shadow(UINT width, UINT height)
	: _width(width)
	, _height(height)
	, _radius(30.0f)
{
	_rtv = new RenderTarget(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	_dsv = new DepthStencil(false, width, height);

	_depthMap = new UIImage(L"Texture");
	_depthMap->scale = { 300, 300, 1 };
	_depthMap->position = { 150, 150, 0 };
	_depthMap->SetSRV(_rtv->GetSRV());

	_viewBuffer = new ViewBuffer();
	_projectionBuffer = new MatrixBuffer();

	_qualityBuffer = new TypeBuffer();
	_sizeBuffer = new SizeBuffer();
	_sizeBuffer->data.size = Vector2(width, height);
}

Shadow::~Shadow()
{
	delete _sizeBuffer;
	delete _qualityBuffer;
	delete _projectionBuffer;
	delete _viewBuffer;
	delete _depthMap;
	delete _dsv;
	delete _rtv;
}

void Shadow::PreRender()
{
	_rtv->Set(_dsv);
	SetViewProjection();
}

void Shadow::Render()
{
	_viewBuffer->SetVSBuffer(11);
	_projectionBuffer->SetVSBuffer(12);

	_qualityBuffer->SetPSBuffer(10);
	_sizeBuffer->SetPSBuffer(11);

	ID3D11ShaderResourceView* srvs[] =
	{
		_rtv->GetSRV()
	};
	Device::Get().GetDeviceContext()->PSSetShaderResources(10, _countof(srvs), srvs);
}

void Shadow::PostRender()
{
	_depthMap->Render();
}

void Shadow::SetViewProjection()
{
	Vector3 lightDir = Environment::Get().GetLight()->data.lights[0].direction;
	lightDir.Normalize();

	Vector3 lightPos = lightDir * -_radius;

	Matrix view = XMMatrixLookAtLH(lightPos, Vector3::Zero, Vector3::Up);
	Matrix projection = XMMatrixOrthographicLH(_radius * 2.0f, _radius * 2.0f, 0.0f, 1000.0f);

	_viewBuffer->Set(view);
	_projectionBuffer->Set(projection);

	_viewBuffer->SetVSBuffer(1);
	_projectionBuffer->SetVSBuffer(2);
}
