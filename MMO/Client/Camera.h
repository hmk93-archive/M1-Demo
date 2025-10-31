#pragma once

#include "ConstBuffer.h"
#include "Transform.h"

class Camera : public Transform
{
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
public:
	Camera();
	~Camera();

	void Update();
	void PostRender();
	
	void SetVS(UINT slot);

	Ray ScreenPointToRay(Vector3 pos);

private:
	void View();

private:
	bool _fpv = true;
	float _moveSpeed = 25.0f;
	float _rotSpeed = 0.1f;

	Matrix _view = Matrix::Identity;

	ViewBuffer* _viewBuffer = nullptr;
};

