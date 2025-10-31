#include "pch.h"
#include "Control.h"

Control::Control()
	: curState{}, oldState{}, mapState{}, wheelValue(0)
{		
}

Control::~Control()
{
}

void Control::Update()
{
	memcpy(oldState, curState, sizeof(oldState));

	//0, 1, 128, 129
	GetKeyboardState(curState);

	for (int i = 0; i < KEYMAX; i++)
	{
		BYTE key = curState[i] & 0x80;
		curState[i] = key ? 1 : 0;

		int old = oldState[i];
		int cur = curState[i];

		if (old == 0 && cur == 1)
			mapState[i] = DOWN;		
		else if (old == 1 && cur == 0)
			mapState[i] = UP;		
		else if (old == 1 && cur == 1)		
			mapState[i] = PRESS;		
		else
			mapState[i] = NONE;		
	}
}

void Control::SetMouse(LPARAM lParam)
{
	float x = (float)LOWORD(lParam);
	float y = (float)HIWORD(lParam);

	if (x <= g_screenWidth)
		mousePos.x = x;
	if (y <= g_screenHeight)
		mousePos.y = y;
}
