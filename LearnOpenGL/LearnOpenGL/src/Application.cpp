#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ErrorHandling.h"

#include <memory>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>

#include "Game.h"

#include "ResourceManager.h"

GLFWwindow* _pWindow = nullptr;

unsigned int width = 1280;
unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

Game* Breakout;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

void Initialize()
{
	glewInit();

	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	_pWindow = glfwCreateWindow(width, height, "Breakout", NULL, NULL);
	if (!_pWindow)
	{
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(_pWindow);

	//Syncs to monitor refresh rate
	glfwSwapInterval(1);

	//glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_pWindow, MouseCallback);
	glfwSetScrollCallback(_pWindow, ScrollCallback);
	glfwSetKeyCallback(_pWindow, KeyCallback);
	glfwSetMouseButtonCallback(_pWindow, MouseButtonCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "[ERROR]: With setting up GLEW" << std::endl;
	}

	Breakout = new Game(width, height, _pWindow);
	Breakout->Init();
}



int main()
{
	Initialize();

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		//Calculate Delta Time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Breakout->ProcessInput(deltaTime);

		Breakout->Update(deltaTime);


		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		Breakout->Render();


		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}


void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Breakout->SetMousePos(glm::vec2(xpos, ypos));
	
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//TODO: Allow for all mouse buttons to be set

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			Breakout->SetMousePressed(true);
		}
		else if (action == GLFW_RELEASE) {
			Breakout->SetMousePressed(false);
		}
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(_pWindow, true);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout->_keys[key] = true;
		else if (action == GLFW_RELEASE)
			Breakout->_keys[key] = false;
	}

}