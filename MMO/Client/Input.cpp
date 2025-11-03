#include "pch.h"
#include "Input.h"

Input::Input()
	: _curState{}
	, _oldState{}
	, _mapState{}
	, _wheelValue(0)
{		
}

Input::~Input()
{
}

void Input::Update()
{
	memcpy(_oldState, _curState, sizeof(_oldState));

	//0, 1, 128, 129
	GetKeyboardState(_curState);

	for (int i = 0; i < KEYMAX; i++)
	{
		BYTE key = _curState[i] & 0x80;
		_curState[i] = key ? 1 : 0;

		int old = _oldState[i];
		int cur = _curState[i];

		if (old == 0 && cur == 1)
			_mapState[i] = DOWN;		
		else if (old == 1 && cur == 0)
			_mapState[i] = UP;		
		else if (old == 1 && cur == 1)		
			_mapState[i] = PRESS;		
		else
			_mapState[i] = NONE;		
	}


	_deltaMousePos.x = (_mousePos.x - _lastMousePos.x);
	_deltaMousePos.y = (_mousePos.y - _lastMousePos.y);

	_lastMousePos = _mousePos;
}

void Input::SetMouse(LPARAM lParam)
{
	float x = (float)LOWORD(lParam);
	float y = (float)HIWORD(lParam);

	if (x <= g_screenWidth)
		_mousePos.x = x;
	if (y <= g_screenHeight)
		_mousePos.y = y;
}
