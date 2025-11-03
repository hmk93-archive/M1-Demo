#include "pch.h"
#include "Camera.h"
#include "Input.h"
#include "Timer.h"
#include "Environment.h"

Camera::Camera()
{
	_viewBuffer = new ViewBuffer();
}

Camera::~Camera()
{
	delete _viewBuffer;
}

void Camera::Update()
{
	switch (mode)
	{
	case Follow:
		FollowMode();
		break;
	case Editor:
		EditorMode();
		break;
	}
}

void Camera::PostRender()
{
	ImGui::Begin("[Camera]");
	ImGui::RadioButton("FOLLOW", (int*)&mode, 0); ImGui::SameLine();
	if (ImGui::RadioButton("EDITOR", (int*)&mode, 1)) position = Vector3::Zero;
	ImGui::Checkbox("FPV", &_fpv);
	ImGui::InputFloat("MoveSpeed", &_moveSpeed, 1.0f, 50.0f);
	ImGui::End();
}

void Camera::SetVS(UINT slot)
{
	_viewBuffer->SetVSBuffer(slot);
}

void Camera::FollowMode()
{
	_yaw += Input::Get().GetDeltaMouse().x * 0.005f;
	_pitch += Input::Get().GetDeltaMouse().y * 0.005f;
	_pitch = clamp(_pitch, _minPitch, _maxPitch);

	if (!_target)
		return;

	Matrix rotMatrix = Matrix::Identity;
	if (_rotDamping > 0.0f)
	{
		if (_target->rotation.y != _destRot)
		{
			_destRot = std::lerp(_destRot, _target->rotation.y + XM_PI, _rotDamping * Timer::Get().GetElapsedTime());
		}

		rotMatrix = XMMatrixRotationY(_destRot);
	}
	else
	{
		// FollowControl();
		// rotMatrix = XMMatrixRotationY(rotY);
	}
	
	Vector3 forward = Vector3::Transform(Vector3::Backward, rotMatrix);
	_destPos = forward * -_distance;
	_destPos += _target->GlobalPos();
	_destPos.y += _height;

	position = Vector3::Lerp(position, _destPos, _moveDamping * Timer::Get().GetElapsedTime());
	// rotation.y = _yaw;

	_view = XMMatrixLookAtLH(position, _target->GlobalPos(), Up());
	_viewBuffer->Set(_view);

	UpdateWorld();
}

void Camera::EditorMode()
{
	if (Input::Get().Down('F'))
		_fpv = !_fpv;
	EditorMove();
	EditorRotate();
	View();
}

void Camera::EditorMove()
{
	if (Input::Get().Press('W'))
		position += Forward() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Input::Get().Press('S'))
		position -= Forward() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Input::Get().Press('A'))
		position -= Right() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Input::Get().Press('D'))
		position += Right() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Input::Get().Press('Q'))
		position -= Up() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Input::Get().Press('E'))
		position += Up() * _moveSpeed * Timer::Get().GetElapsedTime();
}

void Camera::EditorRotate()
{
	if (!_fpv)
		return;

	// Rotation
	float ndcX = Input::Get().GetMouse().x / g_screenWidth * 2.0f - 1.0f;
	float ndcY = -Input::Get().GetMouse().y / g_screenHeight * 2.0f + 1.0f;

	float yaw = ndcX * XM_PI * _rotSpeed;
	float pitch = -ndcY * XM_PIDIV2 * _rotSpeed;

	rotation.x = pitch;
	rotation.y = yaw;
}

void Camera::View()
{
	UpdateWorld();

	Vector3 focus = position + Forward();
	_view = XMMatrixLookAtLH(position, focus, Up());

	_viewBuffer->Set(_view);
}

Ray Camera::ScreenPointToRay(Vector3 pos)
{
	Vector2 screenSize((float)g_screenWidth, (float)g_screenHeight);

	Vector2 point;
	point.x = ((2 * pos.x) / screenSize.x) - 1.0f;
	point.y = (((2 * pos.y) / screenSize.y) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get().GetProjection();

	Matrix temp;
	XMStoreFloat4x4(&temp, projection);

	point.x /= temp._11;
	point.y /= temp._22;

	Ray ray;
	ray.position = position;

	Matrix invView = _view.Invert();

	Vector3 tempPos(point.x, point.y, 1.0f);

	ray.direction = XMVector3TransformNormal(tempPos, invView);
	ray.direction.Normalize();

	return ray;
}

