#include "stdafx.h"
#include "Time.h"

s_clock::time_point Time::previous{};
double Time::lag{};
double Time::elapsed{};

void Time::initialize()
{
	previous = s_clock::now();
}

void Time::update()
{
	auto current = s_clock::now();
	elapsed = (current - previous).count();
	previous = current;
	lag += elapsed;
}
