#pragma once
#include <conio.h>

class Input
{
	static bool olds[256];

public:
	static bool keys[256];
	static bool keyUp[256];
	static bool keyDown[256];

public:
	static void initialize() {}
	static void update();
	static void clear();
};
