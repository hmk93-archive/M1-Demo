#pragma once

#include "ConstBuffer.h"

class MatrixBuffer;
class SamplerState;

class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
		Matrix invMatrix;
	} data;

public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.matrix = Matrix::Identity;
		data.invMatrix = Matrix::Identity;
	}

	void Set(Matrix value)
	{
		data.matrix = value.Transpose();
		data.invMatrix = value.Invert();
	}
};

class Environment
{
	DECLARE_SINGLETON(Environment);
public:
	void Set();

	void SetProjection();
	void SetViewport(UINT width = g_screenWidth, UINT height = g_screenHeight);

	Matrix GetProjection() { return _projection; }
	MatrixBuffer* GetProjectionBuffer() { return _projectionBuffer; }

private:
	void CreatePerspective();
	void CreateView();

private:
	Matrix _projection = Matrix::Identity;
	Matrix _view = Matrix::Identity;

	D3D11_VIEWPORT _viewport = {};

	MatrixBuffer* _projectionBuffer = nullptr;
	ViewBuffer* _viewBuffer = nullptr;

	SamplerState* _samplerState = nullptr;
};

