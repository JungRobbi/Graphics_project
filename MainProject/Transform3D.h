#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "Vector3.h"
class Transform3D : public Component
{
public:

	Vector3 position;

	glm::mat4 mat_Transform;

	void start() {}
	void update() {}
};

