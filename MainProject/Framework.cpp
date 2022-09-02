#include "stdafx.h"
#include "Framework.h"
#include "Time.h"
#include "Input.h"

void Framework::run()
{
	Time::initialize();
	while (true) {
		Time::update();
		Input::update();

		if (Input::keyDown[27])
			return;

		while (Time::lag >= MS_PER_UPDATE) {
			scenes.front()->scene->update();
			Input::clear();
			Time::lag -= MS_PER_UPDATE;
		}

		render();
	}
}
