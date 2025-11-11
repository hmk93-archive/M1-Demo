#pragma once

#include "ConstBuffer.h"
#include "Transform.h"

class ViewBuffer;

class Camera : public Transform
{
public:
	enum CamMode
	{
		Follow,
		Editor
	} mode = Follow;

public:
	Camera();
	~Camera();

	void Update();
	void PostRender();
	
	void SetVS(UINT slot);

	void SetTarget(Transform* target) { _target = target; }

	Ray ScreenPointToRay(Vector3 pos);

	Matrix GetView() { return _view; }

private:
	void FollowMode();
	void EditorMode();

	void EditorMove();
	void EditorRotate();

	void View();

	void MouseWheel();

private:
	bool _fpv = false;
	float _moveSpeed = 50.0f;
	float _rotSpeed = 1.0f;

	Matrix _view = Matrix::Identity;

	ViewBuffer* _viewBuffer = nullptr;

	Transform* _target = nullptr;

	float _rotDamping = 0.0f;
	float _moveDamping = 5.0f;
	float _destRot = 0.0f;

	Vector3 _destPos = Vector3::Zero;
	float _distance = 7.5f;
	float _height = 7.5f;

	float _yaw = 0.0f;
	float _pitch = 0.0f;
	float _minPitch = -XM_PIDIV4;
	float _maxPitch = XM_PIDIV4;
};

