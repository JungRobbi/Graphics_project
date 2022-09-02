#include <gl/glew.h>

#include "stdafx.h"
#include <vector>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <cctype>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderComponent.h"
#include "shader.h"
#include "Transform3D.h"
#include "Texture.h"

void RenderComponent::start()
{
	modelLocation = glGetUniformLocation(s_program[0], "model");
}

void RenderComponent::update()
{
	if (!gameObject->GetComponent<Transform3D>())
		return;
	auto tr = gameObject->GetComponent<Transform3D>()->mat_Transform;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tr));
	glBindTexture(GL_TEXTURE_2D, Texture::texture[num_image]);
	glDrawArrays(GL_TRIANGLES, 0, num_index);
}
