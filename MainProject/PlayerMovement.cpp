#include "PlayerMovement.h"


void PlayerMovement::update()
{
	auto& position = gameObject->GetComponent<Transform3D>()->position;
	if (Input::keyDown['w'])
		position.y -= 0.01;
	if (Input::keyDown['a'])
		position.x -= 0.01;
	if (Input::keyDown['s'])
		position.y += 0.01;
	if (Input::keyDown['d'])
		position.x += 0.01;
}