#include "pch.h"
#include "Camera.h"
#include "Control.h"
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
	if (!_fpv)
		return;

	// Move
	if (Control::Get().Press('W'))
		position += Forward() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Control::Get().Press('S'))
		position -= Forward() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Control::Get().Press('A'))
		position -= Right() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Control::Get().Press('D'))
		position += Right() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Control::Get().Press('Q'))
		position -= Up() * _moveSpeed * Timer::Get().GetElapsedTime();
	if (Control::Get().Press('E'))
		position += Up() * _moveSpeed * Timer::Get().GetElapsedTime();

	// Rotation
	float ndcX = Control::Get().GetMouse().x / g_screenWidth * 2.0f - 1.0f;
	float ndcY = -Control::Get().GetMouse().y / g_screenHeight * 2.0f + 1.0f;

	float yaw = ndcX * XM_PI * _rotSpeed;
	float pitch = -ndcY * XM_PIDIV2 * _rotSpeed;

	rotation.x = pitch;
	rotation.y = yaw;

	View();
}

void Camera::PostRender()
{
	ImGui::Begin("[Camera]");
	ImGui::Checkbox("FPV", &_fpv);
	ImGui::InputFloat("MoveSpeed", &_moveSpeed, 1.0f, 50.0f);
	ImGui::End();
}

void Camera::SetVS(UINT slot)
{
	_viewBuffer->SetVSBuffer(slot);
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
	Vector2 screenSize(g_screenWidth, g_screenHeight);

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
