#include "pch.h"
#include "Environment.h"
#include "Device.h"
#include "GlobalBuffers.h"
#include "SamplerState.h"

Environment::Environment()
{
	CreatePerspective();
	CreateView();

	_samplerState = new SamplerState();
	_samplerState->SetState();
}

Environment::~Environment()
{
	delete _samplerState;
	delete _projectionBuffer;
	delete _viewBuffer;
}

void Environment::Set()
{
	SetViewport();

	_viewBuffer->SetVSBuffer(1);

	SetProjection();

	
}

void Environment::SetProjection()
{
	_projectionBuffer->SetVSBuffer(2);
}

void Environment::SetViewport(UINT width, UINT height)
{
	_viewport.Width = (float)width;
	_viewport.Height = (float)height;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;

	Device::Get().GetDeviceContext()->RSSetViewports(1, &_viewport);
}

void Environment::CreatePerspective()
{
	_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)g_screenWidth / (float)g_screenHeight, 0.1f, 1000.0f);

	_projectionBuffer = new MatrixBuffer();

	_projectionBuffer->Set(_projection);
}

void Environment::CreateView()
{
	Vector3 eyePos = Vector3(0.0f, 0.0f, -10.0f);
	Vector3 eyeDir = Vector3(0.0f, 0.0f, 1.0f);

	_view = XMMatrixLookToLH(eyePos, eyeDir, Vector3(0.0f, 1.0f, 0.0f));

	_viewBuffer = new ViewBuffer();

	_viewBuffer->Set(_view);
}
