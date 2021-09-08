#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "ErrorHandling.h"

#include "Shader.h"
#include "TextureLoader.h"

#include "Camera.h"
#include "Model.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>

#include "Vertices.h"

GLFWwindow* _pWindow = nullptr;

unsigned int width = 1280;
unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

Camera* camera;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void BindTexture(GLenum slot, GLenum target, unsigned int& textureID) {
	GLCall(glActiveTexture(slot));
	GLCall(glBindTexture(target, textureID));
}

void CreateBuffers(unsigned int& vao, unsigned int& vbo, float* vert, unsigned int count) {
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * count, vert, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
}

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
	_pWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!_pWindow)
	{
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(_pWindow);

	//Syncs to monitor refresh rate
	glfwSwapInterval(1);

	glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_pWindow, MouseCallback);
	glfwSetScrollCallback(_pWindow, ScrollCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "[ERROR]: With setting up GLEW" << std::endl;
	}
}

void ProcessInput();

int main(void)
{
	Initialize();

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	unsigned int cubeVao;
	unsigned int vbo;
	CreateBuffers(cubeVao, vbo, cubeVertices, sizeof(cubeVertices) / sizeof(float));

	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	unsigned int quadVAO;
	unsigned int quadVBO;
	CreateBuffers(quadVAO, quadVBO, quadVerts, sizeof(quadVerts) / sizeof(float));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	TextureLoader textureLoader;
	unsigned int crateDiffuse = textureLoader.LoadTexture("Res/Textures/container2.png");
	unsigned int crateSpecular = textureLoader.LoadTexture("Res/Textures/container2_specular.png");
	unsigned int floorDiffuse = textureLoader.LoadTexture("Res/Textures/floor.png");
	unsigned int floorSpecular = textureLoader.LoadTexture("Res/Textures/floorSpecular.png");

	std::vector<std::string> faces{
		"Res/Textures/skybox/right.jpg",
		"Res/Textures/skybox/left.jpg",
		"Res/Textures/skybox/top.jpg",
		"Res/Textures/skybox/bottom.jpg",
		"Res/Textures/skybox/front.jpg",
		"Res/Textures/skybox/back.jpg",
	};

	unsigned int skybox = textureLoader.LoadCubemap(faces);

	Shader* lightShader = new Shader("Res/Shaders/LightShader.vert", "Res/Shaders/LightShader.frag");
	Shader* skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");
	Shader* advancedLightingShader = new Shader("Res/Shaders/AdvancedLighting.vert", "Res/Shaders/AdvancedLighting.frag");

	glm::vec3 lightPos(1.0,1.0,1.0);

	camera = new Camera(_pWindow, width, height, 45.0f);

	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun(glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);

	std::vector<PointLight> pointLights;
	pointLights.push_back(PointLight(glm::vec3(0.2f, 0.2f, 2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));


	for (int i = 0; i < 4; i++) {
		advancedLightingShader->SetFloat3(("lightPosition[" + std::to_string(i) + "]").c_str(), pointLights[i].position);
		advancedLightingShader->SetFloat3(("lightColors[" + std::to_string(i) + "]").c_str(), pointLights[i].diffuse);
	}

	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float fps = 1.0 / deltaTime;

		glfwSetWindowTitle(_pWindow, ("[OpenGL Experiments] FPS: " + std::to_string(fps)).c_str());

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		camera->Update(deltaTime);

		glm::mat4 view = camera->ViewMat();
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 projection = camera->ProjMat();

		//Draw Skybox
		glDepthMask(false);
		glBindVertexArray(skyboxVAO);
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->ViewMat()));
		skyboxShader->SetMatrix4("projection", projection);
		skyboxShader->SetMatrix4("view", skyboxView);
		BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(true);


		//Draw Light
		glBindVertexArray(cubeVao);
		lightShader->SetMatrix4("view", view);
		lightShader->SetMatrix4("projection", projection);
		for (int i = 0; i < 4; i++) {
			model = glm::mat4(1.0);
			model = glm::translate(model, pointLights[i].position);
			model = glm::scale(model, glm::vec3(0.2));
			
			lightShader->SetMatrix4("model", model);
			lightShader->SetFloat3("lightColor", pointLights[i].diffuse);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}




		advancedLightingShader->SetInt("diffuseTexture", 0);
		advancedLightingShader->SetFloat3("viewPos", camera->Position());

		advancedLightingShader->SetMatrix4("view", view);
		advancedLightingShader->SetMatrix4("projection", projection);



		//Draw Cubes
		GLCall(glBindVertexArray(cubeVao));
		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, crateDiffuse);
		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			advancedLightingShader->SetMatrix4("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		//move floor plane
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.25f, 0.0f, -12.5f));
		model = glm::scale(model, glm::vec3(25.0f, 1.0f, 25.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,0.0f));
		advancedLightingShader->SetMatrix4("model", model);

		//Draw Floor
		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, floorDiffuse);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();

		ProcessInput();
	}

	lightShader->CleanupShader();

	delete lightShader;

	lightShader = nullptr;

	glfwTerminate();
	return 0;
}

void ProcessInput()
{
	if (glfwGetKey(_pWindow, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(_pWindow, true);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera->ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessScrollMovement(yoffset);
}