#pragma once

class MatrixBuffer;
class SamplerState;
class Camera;

class Environment
{
	DECLARE_SINGLETON(Environment);
public:
	void Set();
	void PostRender();

	void SetProjection();
	void SetViewport(UINT width = g_screenWidth, UINT height = g_screenHeight);

	Camera* GetMainCamera() { return _mainCamera; }

	Matrix GetProjection() { return _projection; }
	MatrixBuffer* GetProjectionBuffer() { return _projectionBuffer; }

	void CreatePerspective();

private:
	Matrix _projection = Matrix::Identity;
	Matrix _view = Matrix::Identity;

	D3D11_VIEWPORT _viewport = {};

	Camera* _mainCamera = nullptr;

	MatrixBuffer* _projectionBuffer = nullptr;

	SamplerState* _samplerState = nullptr;
};

