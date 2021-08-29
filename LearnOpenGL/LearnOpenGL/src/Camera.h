#pragma once

#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(GLFWwindow* window, int width, int height, float fov);


	void Update(float dt);

	float* GetProj() {
		return glm::value_ptr(proj);
	}

	float* GetView() {
		return glm::value_ptr(view);
	}

	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessScrollMovement(float yOffset);
	
private:
	void PollInput(float dt);

private:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;


	glm::vec3 cameraRight;

	glm::mat4 proj;
	glm::mat4 view;

	float fov = 45.0f;

	int width;
	int height;

	float pitch = 0.0f;
	float yaw = -90.0f;

	GLFWwindow* _pWindow;
};

