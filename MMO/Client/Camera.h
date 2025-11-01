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
	enum CamMode
	{
		Follow,
		Editor
	} mode;

public:
	Camera();
	~Camera();

	void Update();
	void PostRender();
	
	void SetVS(UINT slot);

	void SetTarget(Transform* target) { _target = target; }

	Ray ScreenPointToRay(Vector3 pos);

private:
	void FollowMode();
	void EditorMode();

	void EditorMove();
	void EditorRotate();

	void View();

private:
	bool _fpv = false;
	float _moveSpeed = 25.0f;
	float _rotSpeed = 0.1f;

	Matrix _view = Matrix::Identity;

	ViewBuffer* _viewBuffer = nullptr;

	Transform* _target = nullptr;

	float _rotDamping = 0.0f;
	float _moveDamping = 5.0f;
	float _destRot = 0.0f;

	Vector3 _destPos = Vector3::Zero;
	float _distance = 60.0f;
	float _height = 60.0f;
};

