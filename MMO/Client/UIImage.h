#pragma once

#include "Transform.h"

class MatrixBuffer;
class DepthStencilState;

class UIImage : public Transform
{
	using VertexType = VertexUV;

public:
	enum Pivot
	{
		Center,
	} pivot = Center;

public:
	UIImage(wstring shaderFile, UIImage::Pivot inPivot = UIImage::Pivot::Center, bool blendAlpha = false, bool depthEnable = false);
	virtual ~UIImage();

	void Update();
	void Render();

	void SetSRV(ID3D11ShaderResourceView* srv) { _srv = srv; }
	Material* GetMaterial() { return _material; };
	Mesh* GetMesh() { return _mesh; }

private:
	void CreateMesh();
	void CreateMatrix();

protected:
	Material* _material = nullptr;
	Mesh* _mesh = nullptr;

	vector<VertexType> _vertices = {};

	Matrix _view = Matrix::Identity;
	Matrix _ortho = Matrix::Identity;

	MatrixBuffer* _viewBuffer = nullptr;
	MatrixBuffer* _orthoBuffer = nullptr;

	DepthStencilState* _depthMode[2] = {};

	ComPtr<ID3D11ShaderResourceView> _srv;
};

