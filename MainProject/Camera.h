#pragma once
#include "stdafx.h"

#include "Component.h"
#include "Transform3D.h"
#include "GameObject.h"

enum Camera_state { FIRST_VIEW, FIRST_TO_TOP, TOP_VIEW, TOP_TO_FIRST };

class Camera : public Component
{

public:
	unsigned int viewLocation;
	unsigned int projLocation;

	glm::mat4 Vw;
	glm::mat4 Pj;

	glm::vec3 top_pos = glm::vec3(0.0f, 19.0f, 0.0f);
	glm::vec3 top_dir = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 top_up = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 c_pos3;
	glm::vec3 c_dir3;
	glm::vec3 c_up = glm::vec3(0.0f, 1.0f, 0.0f);

	float fpsy = 0.0f;
	float fpsup = 0.0f;

	Camera_state state = FIRST_VIEW;

public:

	void start();
	void update();
};

