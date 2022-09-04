#pragma once
#include "stdafx.h"

#include "Component.h"
#include "Transform3D.h"
#include "GameObject.h"

class Camera : public Component
{

public:
	unsigned int viewLocation;
	unsigned int projLocation;

	glm::mat4 Vw;
	glm::mat4 Cp;
	glm::mat4 Pj;

	float fpsy = 0.0f;
	float fpsup = 0.0f;

public:


	void start();
	void update();
};

