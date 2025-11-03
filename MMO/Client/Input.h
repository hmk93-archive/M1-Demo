#pragma once

#define KEYMAX 255

class Input 
{
	DECLARE_SINGLETON(Input);
private:
	enum
	{
		NONE,
		DOWN,
		UP,
		PRESS
	};
public:
	void Update();

	bool Down(UINT key) { return _mapState[key] == DOWN; }
	bool Up(UINT key) { return _mapState[key] == UP; }
	bool Press(UINT key) { return _mapState[key] == PRESS; }

	Vector3 GetMouse() { return _mousePos; }
	void SetMouse(LPARAM lParam);

	float GetWheel() { return _wheelValue; }
	void SetWheel(float value) { _wheelValue = value; }

	Vector3 GetDeltaMouse() { return _deltaMousePos; }

private:
	BYTE _curState[KEYMAX];
	BYTE _oldState[KEYMAX];
	BYTE _mapState[KEYMAX];

	Vector3 _mousePos = {};
	float _wheelValue = 0.0f;

	Vector3 _lastMousePos = {};
	Vector3 _deltaMousePos = {};
};