#include "pch.h"
#include "Player.h"

Player::Player(string file)
	: ModelAnimator(file + "/" + file)
{
	SetShader(L"Default");

	ReadClip(file + "/Idle0");
	ReadClip(file + "/Run0");
	
	SetAnimation(Run);
}

Player::~Player()
{
}

void Player::Update()
{
	ModelAnimator::Update();
}

void Player::Render()
{
	SetWorldBuffer();
	ModelAnimator::Render();
}

void Player::PostRender()
{

}

void Player::SetAnimation(PlayerAnimState value, float speed)
{
	if (state != value)
	{
		state = value;
		PlayClip(state, speed);
	}
}
