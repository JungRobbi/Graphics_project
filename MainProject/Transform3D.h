#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "Vector3.h"
#include "GameObject.h"

#include "Collide.h"
class Transform3D : public Component
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	float pitch = 0.0f; // x °¢
	float yaw = 0.0f; // y°¢
	float roll = 0.0f; // z°¢

	glm::mat4 mat_Transform;

	void start() 
	{

	}
	void update() 
	{
		position += velocity;

		mat_Transform = glm::mat4(1.0f);

		mat_Transform = glm::translate(mat_Transform, position);

		mat_Transform = glm::rotate(mat_Transform, (float)glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
		mat_Transform = glm::rotate(mat_Transform, (float)glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		mat_Transform = glm::rotate(mat_Transform, (float)glm::radians(roll), glm::vec3(1.0f, 0.0f, 0.0f));

		mat_Transform = glm::scale(mat_Transform, scale);

	}
};

