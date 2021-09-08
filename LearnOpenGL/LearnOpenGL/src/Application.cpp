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
	unsigned int floorTexture = textureLoader.LoadTexture("Res/Textures/floor.png");

	std::vector<std::string> faces{
		"Res/Textures/skybox/right.jpg",
		"Res/Textures/skybox/left.jpg",
		"Res/Textures/skybox/top.jpg",
		"Res/Textures/skybox/bottom.jpg",
		"Res/Textures/skybox/front.jpg",
		"Res/Textures/skybox/back.jpg",
	};

	unsigned int skybox = textureLoader.LoadCubemap(faces);

	Shader* objectShader = new Shader("Res/Shaders/BasicShader.vert", "Res/Shaders/BasicShader.frag");
	Shader* lightShader = new Shader("Res/Shaders/LightShader.vert", "Res/Shaders/LightShader.frag");
	Shader* modelShader = new Shader("Res/Shaders/ModelShader.vert", "Res/Shaders/ModelShader.frag");
	Shader* skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");
	Shader* instancedShader = new Shader("Res/Shaders/InstancedModel.vert", "Res/Shaders/InstancedModel.frag");


	Model planet("Res/Models/planet/planet.obj");

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	camera = new Camera(_pWindow, width, height, 45.0f);

	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun(glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);

	std::vector<PointLight> pointLights;
	pointLights.push_back(PointLight(glm::vec3(0.7f, 0.2f, 2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));


	objectShader->SetDirectionalLight("u_dirLight", sun);
	objectShader->SetInt("u_material.diffuse", 0);
	objectShader->SetInt("u_material.specular", 1);
	objectShader->SetFloat("u_material.shininess", 32.0f);

	lightShader->SetFloat3("u_lightColor", glm::value_ptr(lightDiffuse));

	modelShader->SetDirectionalLight("u_dirLight", sun);
	modelShader->SetFloat("u_material.shininess", 32.0f);


	glEnable(GL_MULTISAMPLE);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float fps = 1.0 / deltaTime;

		glfwSetWindowTitle(_pWindow, ("[OpenGL Experiments] FPS: " + std::to_string(fps)).c_str());

		for (GLuint i = 0; i < 4; i++) {
			std::string number = std::to_string(i);
			std::string base = "u_pointLight[" + number + "]";
			objectShader->SetPointLight(base, pointLights[i]);
		}


		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		camera->Update(deltaTime);

		glm::mat4 view = camera->ViewMat();
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 projection = camera->ProjMat();

		glDepthMask(false);
		glBindVertexArray(skyboxVAO);
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->ViewMat()));
		skyboxShader->SetMatrix4("projection", camera->Proj());
		skyboxShader->SetMatrix4("view", skyboxView);
		BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(true);


		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, crateDiffuse);
		BindTexture(GL_TEXTURE1, GL_TEXTURE_2D, crateSpecular);
		objectShader->SetFloat3("u_viewPos", camera->Position());
		objectShader->SetMatrix4("u_view", view);
		objectShader->SetMatrix4("u_projection", camera->Proj());

		//Draw Cubes
		GLCall(glBindVertexArray(cubeVao));
		BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skybox);
		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			objectShader->SetMatrix4("u_model", glm::value_ptr(model));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		//Draw Lights
		lightShader->SetMatrix4("u_view", view);
		lightShader->SetMatrix4("u_projection", camera->Proj());
		for (int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLights[i].position);
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			lightShader->SetMatrix4("u_model", glm::value_ptr(model));

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
		
		//Draw Planet
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 2.35f, -7.0f));
		modelShader->SetFloat3("u_viewPos", camera->Position());
		modelShader->SetMatrix4("view", view);
		modelShader->SetMatrix4("projection", camera->Proj());
		modelShader->SetMatrix4("model", glm::value_ptr(model));
		planet.Draw(modelShader);


		objectShader->BindShaderProgram();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.25f, 0.0f, -12.5f));
		model = glm::scale(model, glm::vec3(25.0f, 1.0f, 25.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,0.0f));
		

		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, floorTexture);
		BindTexture(GL_TEXTURE1, GL_TEXTURE_2D, crateSpecular);
		objectShader->SetMatrix4("u_model", model);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();

		ProcessInput();
	}

	objectShader->CleanupShader();
	lightShader->CleanupShader();

	delete lightShader;
	delete objectShader;

	lightShader = nullptr;
	objectShader = nullptr;

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