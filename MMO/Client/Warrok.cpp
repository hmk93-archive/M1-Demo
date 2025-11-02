#include "pch.h"
#include "Warrok.h"
#include "SphereCollider.h"

Warrok::Warrok(string file)
	: ModelAnimators(file + "/" + file)
{
	SetShader(L"Instancing");

	ReadClip(file + "/Idle0");

	SetAnimation(Idle);
}

Warrok::~Warrok()
{
	for(Collider* collider : mainCollider)
		delete collider;
}

void Warrok::Update()
{
	for (UINT i = 0; i < _drawCount; i++)
		mainCollider[i]->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	ModelAnimators::Update();
}

void Warrok::Render()
{
	ModelAnimators::Render();

	for (UINT i = 0; i < _drawCount; i++)
		mainCollider[i]->Render();
}

void Warrok::PostRender()
{

}

void Warrok::Hit(UINT instanceID, UINT damage)
{
	mainCollider[instanceID]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Warrok::SetAnimation(WarrokAnimState value, float speed)
{
	if (state != value)
	{
		state = value;
		PlayClip(0, state, speed);
	}
}
