#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "Component.h"
#include "stdafx.h"

class RenderComponent :
    public Component
{
	unsigned int modelLocation;
	int num_index;
	int num_image;
public:
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

