#include "LavaMove.h"


void LavaMove::start()
{

}

void LavaMove::update()
{
	if (updown) {
		gameObject->GetComponent<Transform3D>()->scale.z += MoveSpeed;
		count++;
		if (count > 30) {
			updown = false;
		}
	}
	else {
		gameObject->GetComponent<Transform3D>()->scale.z -= MoveSpeed;
		count--;
		if (count < -30) {
			updown = true;
		}
	}
}