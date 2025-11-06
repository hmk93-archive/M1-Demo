#pragma once

class RenderTarget;
class DepthStencil;
class MatrixBuffer;
class ViewBuffer;
class SizeBuffer;
class TypeBuffer;
class UIImage;

class Shadow
{
public:
	Shadow(UINT width = 2048, UINT height = 2048);
	~Shadow();

	void PreRender();
	void Render();
	void PostRender();

private:
	void SetViewProjection();

private:
	UINT _width = 0;
	UINT _height = 0;
	float _radius = 0.0f;

	RenderTarget* _rtv = nullptr;
	DepthStencil* _dsv = nullptr;

	ViewBuffer* _viewBuffer = nullptr;
	MatrixBuffer* _projectionBuffer = nullptr;

	UIImage* _depthMap = nullptr;

	TypeBuffer* _qualityBuffer = nullptr;
	SizeBuffer* _sizeBuffer = nullptr;
};

