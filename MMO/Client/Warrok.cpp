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
	delete mainCollider;
}

void Warrok::Update()
{
	ModelAnimators::Update();
}

void Warrok::Render()
{
	ModelAnimators::Render();

	mainCollider->Render();
}

void Warrok::PostRender()
{

}

void Warrok::SetAnimation(WarrokAnimState value, float speed)
{
	if (state != value)
	{
		state = value;
		PlayClip(state, speed);
	}
}
