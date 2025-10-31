#pragma once

#include "ModelAnimator.h"
#include "Transform.h"

class Player : public ModelAnimator, public Transform
{
	enum PlayerAnimState
	{
		Idle,
		Run,
		End
	} state;
public:
	Player(string file);
	~Player();

	void Update();

	void Render();
	void PostRender();

private:
	void SetAnimation(PlayerAnimState value, float speed = 1.0f);
};

