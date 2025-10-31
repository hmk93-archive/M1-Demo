#pragma once

#define KEYMAX 255

class Control 
{
	DECLARE_SINGLETON(Control);
private:
	enum {
		NONE,
		DOWN,
		UP,
		PRESS
	};

public:

	void Update();

	bool Down(UINT key) { return mapState[key] == DOWN; }
	bool Up(UINT key) { return mapState[key] == UP; }
	bool Press(UINT key) { return mapState[key] == PRESS; }

	Vector3 GetMouse() { return mousePos; }
	void SetMouse(LPARAM lParam);

	float GetWheel() { return wheelValue; }
	void SetWheel(float value) { wheelValue = value; }

private:
	BYTE curState[KEYMAX];
	BYTE oldState[KEYMAX];
	BYTE mapState[KEYMAX];

	Vector3 mousePos;
	float wheelValue;
};