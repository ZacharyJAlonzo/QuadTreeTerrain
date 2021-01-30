#pragma once
#include <glm/glm.hpp>

class Camera
{

public:
	Camera(glm::vec3 pos);
	void UpdateCamera(glm::vec3 front, glm::vec3 pos);

	glm::mat4 projection;
	glm::mat4 view;

	float lastX = 0.0;
	float lastY = 0.0;
	float yaw = 0.0;
	float pitch = 0.0;

	glm::vec3 CameraPos;
	glm::vec3 CameraFront;
	glm::vec3 CameraUp;
	glm::vec3 CameraRight;

	float speed = 1.1;

private:


	


};