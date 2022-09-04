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
	glm::vec3 direction;

	glm::vec3 rotate;


	glm::mat4 mat_Transform;



	void start() 
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
		rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		mat_Transform = glm::mat4(1.0f);
	}
	void update() 
	{
		mat_Transform = glm::mat4(1.0f);
		mat_Transform = glm::translate(mat_Transform, position);
		mat_Transform = glm::scale(mat_Transform, scale);
	}
};

