#include "stdafx.h"
#include "Input.h"

bool Input::olds[256];

bool Input::keys[256];
bool Input::keyUp[256];
bool Input::keyDown[256];

void Input::update()
{
	while (_kbhit())
		keys[_getch()] = true;

	for (int i = 0; i < 256; ++i)
	{
		if (keys[i] == true && olds[i] == false)
			keyUp[i] = true;
		if (keys[i] == false && olds[i] == true)
			keyDown[i] = true;
	}
}

void Input::clear()
{
	memcpy(olds, keys, 256);
	memset(keys, 0, 256);
	memset(keyUp, 0, 256);
	memset(keyDown, 0, 256);
}