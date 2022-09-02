#pragma once
#include "Component.h"
#include "Vector3.h"
class Transform3D : public Component
{
public:

	Vector3 position;

	void start() {}
	void update() {}
};

