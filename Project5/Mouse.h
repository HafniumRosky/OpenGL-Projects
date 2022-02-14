#ifndef MOUSE_H
#define MOUSE_H

#include <GL/glut.h>

class Mouse
{
public:
	Mouse() = default;
	Mouse(Mouse&& moveFrom) noexcept;
	Mouse& operator= (Mouse&& moveFrom) noexcept;

	Mouse(Mouse const&) = delete;
	Mouse& operator=(Mouse const&) = delete;

	enum ChangeMode
	{
		MODE_ABSOLUTE = 0,
		MODE_RELATIVE,
	};

	struct State
	{
		bool    leftButton;
		bool    middleButton;
		bool    rightButton;
		bool    xButton1;
		bool    xButton2;
		int     x;
		int     y;
		int		deltaX;
		int		deltaY;
		int     scrollWheelValue;
		ChangeMode    positionMode;
	};

	State m_mouseState;

	class ButtonStateTracker
	{
	public:
		enum ButtonState
		{
			UP = 0,         // Button is up
			HELD = 1,       // Button is held down
			RELEASED = 2,   // Button was just released
			PRESSED = 3,    // Buton was just pressed
		};

		ButtonState leftButton;
		ButtonState middleButton;
		ButtonState rightButton;
		ButtonState xButton1;
		ButtonState xButton2;
	};

	void initMouse()
	{
		m_mouseState.positionMode = ChangeMode::MODE_RELATIVE;
		m_mouseState.x = 960;
		m_mouseState.y = 540;
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(m_mouseState.x, m_mouseState.y);
	}

	void MouseMotion(int x, int y)
	{
		if (m_mouseState.positionMode == ChangeMode::MODE_RELATIVE)
		{
			glutWarpPointer(m_mouseState.x, m_mouseState.y);
			m_mouseState.deltaX = x - m_mouseState.x;
			m_mouseState.deltaY = y - m_mouseState.y;
		}
	}

	void MouseFunction(int button, int state, int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			m_mouseState.leftButton = true;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			m_mouseState.leftButton = false;
	}
};

#endif