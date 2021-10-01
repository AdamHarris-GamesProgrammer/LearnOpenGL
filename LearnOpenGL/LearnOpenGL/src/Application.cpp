#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ErrorHandling.h"

#include <memory>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>

#include "Breakout.h"

#include "ResourceManager.h"
#include "Input.h"
#include "Time.h"

GLFWwindow* _pWindow = nullptr;

unsigned int width = 1280;
unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

Breakout* breakout;

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

	Input::_pWindow = _pWindow;

	/* Make the window's context current */
	glfwMakeContextCurrent(_pWindow);

	//Syncs to monitor refresh rate
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(_pWindow, MouseCallback);
	glfwSetScrollCallback(_pWindow, ScrollCallback);
	glfwSetKeyCallback(_pWindow, KeyCallback);
	glfwSetMouseButtonCallback(_pWindow, MouseButtonCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "[ERROR]: With setting up GLEW" << std::endl;
	}

	breakout = new Breakout(width, height, _pWindow);
}



int main()
{
	Initialize();

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		Time::Tick();
		//Calculate Delta Time
		breakout->ProcessInput();

		breakout->Update();

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		//Left Bottom
		//glViewport(0, 0, halfWidth, halfHeight);
		//Breakout->RenderScene();

		//Left Top
		//glViewport(0, halfHeight, halfWidth, halfHeight);
		breakout->RenderGame();

		//Right
		//glViewport(halfWidth, 0, halfWidth, height);
		//Breakout->RenderSceneUI();

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
	Input::SetMousePosition(glm::vec2(xpos, ypos));
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Input::SetMouseButton((MouseButton)button, (InputState)action);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(_pWindow, true);
	}

	if (key >= 0 && key < 1024)
	{
		Input::SetKey((KeyCode)key, (InputState)action);
	}
}