#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "Vector3.h"
class Transform3D : public Component
{
public:
	glm::vec3 position;
	glm::vec3 scale;


	glm::mat4 mat_Transform;



	void start() 
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		mat_Transform = glm::mat4(1.0f);
	}
	void update() {}
};

