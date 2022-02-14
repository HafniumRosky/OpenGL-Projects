#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard
{
public:

	struct State
	{
		bool W = false;
		bool S = false;
		bool A = false;
		bool D = false;
		bool V = false;
		bool Q = false;
		bool E = false;
		bool R = false;
		bool F = false;
		bool N = false;
		bool ESC = false;
	};


	State keyState;

	void KeyDown(unsigned char key, int x, int y)
	{
		if (key == 'w')
			keyState.W = true;
		if (key == 's')
			keyState.S = true;
		if (key == 'a')
			keyState.A = true;
		if (key == 'd')
			keyState.D = true;
		if (key == 'v')
			keyState.V = true;
		if (key == 'q')
			keyState.Q = true;
		if (key == 'e')
			keyState.E = true;
		if (key == 'r')
			keyState.R = true;
		if (key == 'f')
			keyState.F = true;
		if (key == 'n')
			keyState.N = true;
		if (key == 27)
			keyState.ESC = true;
	}

	void KeyUp(unsigned char key, int x, int y)
	{
		if (key == 'w')
			keyState.W = false;
		if (key == 's')
			keyState.S = false;
		if (key == 'a')
			keyState.A = false;
		if (key == 'd')
			keyState.D = false;
		if (key == 'v')
			keyState.V = false;
		if (key == 'q')
			keyState.Q = false;
		if (key == 'e')
			keyState.E = false;
		if (key == 'r')
			keyState.R = false;
		if (key == 'f')
			keyState.F = false;
		if (key == 'n')
			keyState.N = false;
		if (key == 27)
			keyState.ESC = false;
	}
};

#endif