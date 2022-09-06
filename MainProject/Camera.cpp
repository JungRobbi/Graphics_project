#include "Camera.h"


void Camera::start()
{
	Vw = glm::mat4(1.0f);
	Cp = glm::mat4(1.0f);
	Pj = glm::mat4(1.0f);
}

void Camera::update()
{
	glm::vec3 c_pos3 = gameObject->GetComponent<Transform3D>()->position;
	glm::vec3 c_dir3 = gameObject->GetComponent<Transform3D>()->direction;

	
	Vw = glm::mat4(1.0f);
	Cp = glm::mat4(1.0f);

	Cp = glm::rotate(Cp, (float)glm::radians(fpsy), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec4 c_pos4 = glm::vec4(c_pos3, 0.0f);
	// glm::vec4 c_dir4 = glm::vec4(c_dir3, 0.0f) * Cp;
	glm::vec3 c_up = glm::vec3(0.0f, 1.0f, 0.0f);

	Vw = glm::lookAt(c_pos3, c_dir3, c_up);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);

	Pj = glm::mat4(1.0f);
	Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 100.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);
}