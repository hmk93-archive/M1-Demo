#include "pch.h"
#include "Environment.h"
#include "Device.h"
#include "GlobalBuffers.h"
#include "SamplerState.h"
#include "Camera.h"

Environment::Environment()
{
	CreatePerspective();

	_samplerState = new SamplerState();
	_samplerState->SetState();

	_mainCamera = new Camera();
	_mainCamera->position = Vector3(0.0f, 5.0f, -5.0f);
}

Environment::~Environment()
{
	delete _mainCamera;
	delete _samplerState;
	delete _projectionBuffer;
}

void Environment::Set()
{
	SetViewport();
	SetProjection();
	_mainCamera->SetVS(1);
}

void Environment::PostRender()
{
	_mainCamera->PostRender();
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
	if (_projectionBuffer)
		delete _projectionBuffer;

	_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)g_screenWidth / (float)g_screenHeight, 0.1f, 1000.0f);

	_projectionBuffer = new MatrixBuffer();

	_projectionBuffer->Set(_projection);
}
