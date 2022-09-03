#pragma once
#include "stdafx.h"

#include <stdio.h>
#include <memory.h>
#include <Windows.h>


//const int OUTPUT_WIDTH{ 15 };
//const int OUTPUT_HEIGHT{ 13 };

class Output
{
	//static char screen[OUTPUT_HEIGHT][OUTPUT_WIDTH * 2 + 1];

public:
	static void draw(int x, int y, char const* const str, int size = 2)
	{
		//memcpy(&screen[y][x * 2], str, size);
	}
	static void render();
};
