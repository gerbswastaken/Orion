#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

// For Keyboard Input and Output:

// Each keyboard button event has 2 pieces of information that we need:
struct KeyInputState
{
	bool is_down;
	bool is_changed;
};

enum KEY
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_SPACE,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_CTRL,

	KEY_COUNT
};

struct Input
{
	KeyInputState key_input[KEY::KEY_COUNT];
	int mouse_dx = 0;
	int mouse_dy = 0;
};

#endif

