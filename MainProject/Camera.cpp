#include "Camera.h"


void Camera::start()
{
	Vw = glm::mat4(1.0f);
	Pj = glm::mat4(1.0f);
}

void Camera::update()
{

	Vw = glm::mat4(1.0f);
	Pj = glm::mat4(1.0f);

	if (state == FIRST_VIEW) {
		c_pos3 = gameObject->GetComponent<Transform3D>()->position;
		c_dir3 = gameObject->GetComponent<Transform3D>()->direction;

	}
	else if (state == FIRST_TO_TOP) {
		glm::vec3 to_top_pos = (top_pos - gameObject->GetComponent<Transform3D>()->position) / 50.0f;
		glm::vec3 to_top_dir = (top_dir - gameObject->GetComponent<Transform3D>()->direction) / 50.0f;
		glm::vec3 to_top_up = (top_up - gameObject->GetComponent<Transform3D>()->direction) / 50.0f;

		c_pos3 += to_top_pos;
		c_dir3 += to_top_dir;
		c_up += to_top_up;

		if (c_pos3.y >= 18.8f) {
			//c_pos3 = top_pos;
			c_dir3 = top_dir;
			//c_up = top_up;
			state = TOP_VIEW;
		}
	}
	else if (state == TOP_TO_FIRST) {
		glm::vec3 to_first_pos = (gameObject->GetComponent<Transform3D>()->position - top_pos) / 50.0f;
		glm::vec3 to_first_dir = (gameObject->GetComponent<Transform3D>()->direction - top_dir) / 50.0f;
		glm::vec3 to_first_up = (gameObject->GetComponent<Transform3D>()->direction - top_up) / 50.0f;

		c_pos3 += to_first_pos;
		c_dir3 += to_first_dir;
		c_up += to_first_up;


		if (c_pos3.y <= 0.0f) {
			c_pos3 = gameObject->GetComponent<Transform3D>()->position;
			c_dir3 = gameObject->GetComponent<Transform3D>()->direction;
			c_up = glm::vec3(0.0f, 1.0f, 0.0f);
			state = FIRST_VIEW;
		}
	}

	Vw = glm::lookAt(c_pos3, c_dir3, c_up);
	Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 100.0f);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);
}