#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera(glm::vec3 pos)
{
	projection = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 1000.0);
	CameraPos = pos;
	CameraFront = glm::vec3(0, 0, 1);

	CameraRight = glm::normalize(glm::cross(CameraFront, glm::vec3(0, 1, 0)));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}

void Camera::UpdateCamera(glm::vec3 front, glm::vec3 pos)
{
	CameraPos = pos;
	CameraFront = glm::normalize(front);
	CameraRight = glm::normalize(glm::cross(CameraFront, glm::vec3(0, 1, 0)));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));

	view = glm::mat4(glm::vec4(CameraRight, 0), glm::vec4(CameraUp, 0), glm::vec4(CameraFront, 0), glm::vec4(CameraPos, 1));
	view = glm::inverse(view);
}
