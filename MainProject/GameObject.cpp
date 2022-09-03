#include "GameObject.h"
#include "Transform3D.h"

GameObject::GameObject()
{
	Scene::scene->creationQueue.push(this);
	
}

void GameObject::render()
{
	if (!GetComponent<Transform3D>())
		return;
	auto tr = GetComponent<Transform3D>()->mat_Transform;

	modelLocation = glGetUniformLocation(s_program, "model");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tr));

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, num_index);
}