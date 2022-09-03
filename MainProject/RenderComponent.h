#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform3D.h"
#include "GameObject.h"
#include "Component.h"

class RenderComponent :
    public Component
{
	unsigned int modelLocation;
	int num_index;
	int num_image;

public:
	GLint* p_VAO;
	GLint* p_texture;
	GLuint* p_s_program;

	void start();
	void update();

	void SetNumIndex(int num)
	{
		num_index = num;
	}
	void SetNumImage(int num)
	{
		num_image = num;
	}
};

