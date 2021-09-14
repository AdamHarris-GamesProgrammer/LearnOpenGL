#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "ErrorHandling.h"

#include "Shader.h"

#include "Camera.h"
#include "Model.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>

#include "Vertices.h"

#include "ResourceManager.h"

GLFWwindow* _pWindow = nullptr;

unsigned int width = 1280;
unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

Camera* camera;

std::vector<PointLight> pointLights;
glm::vec3 lightPos(0.0, 0.5, -2.0);

Texture2D floorDiffuse;
Texture2D crateDiffuse;


void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void RenderCube();
void RenderQuad();
void RenderScene(Shader* shader);
void RenderLights();
void ProcessInput();


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

	glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_pWindow, MouseCallback);
	glfwSetScrollCallback(_pWindow, ScrollCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "[ERROR]: With setting up GLEW" << std::endl;
	}
}



int main(void)
{
	Initialize();

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


	camera = new Camera(_pWindow, width, height, 45.0f);

	Shader depthCubeShader = ResourceManager::LoadShader("Res/Shaders/DepthCube.vert", "Res/Shaders/DepthCube.frag", "Res/Shaders/DepthCube.geom", "depthCube");
	Shader shadowShader = ResourceManager::LoadShader("Res/Shaders/ShadowMapping.vert", "Res/Shaders/ShadowMapping.frag", nullptr, "shadowMap");

	ResourceManager::LoadTexture("Res/Textures/Container2.png", true, "crateDiffuse");
	ResourceManager::LoadTexture("Res/Textures/floor.png", true, "floorDiffuse");

	crateDiffuse = ResourceManager::GetTexture("crateDiffuse");
	floorDiffuse = ResourceManager::GetTexture("floorDiffuse");


	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun(glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);


	pointLights.push_back(PointLight(glm::vec3(0.2f, 0.2f, 2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));


	glEnable(GL_DEPTH_TEST);

		//Calculate data for the shadow rendering
	float near_plane = 1.0f, far_plane = 37.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));



	shadowShader.SetFloat3("lightPos", lightPos);
	shadowShader.SetFloat("far_plane", far_plane);
	shadowShader.SetInt("diffuseTexture", 0);
	shadowShader.SetInt("depthMap", 1);
	shadowShader.SetInt("shadows", 1);


	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		//Calculate Delta Time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Calculate FPS
		float fps = 1.0 / deltaTime;
		glfwSetWindowTitle(_pWindow, ("[OpenGL Experiments] FPS: " + std::to_string(fps)).c_str());

		//Recalculate camera values
		camera->Update(deltaTime);


		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		//First Pass: Render to the depth texture
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < 6; ++i) {
			depthCubeShader.SetMatrix4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
			depthCubeShader.SetFloat("far_plane", far_plane);
			depthCubeShader.SetFloat3("lightPos", lightPos);
		}


		glCullFace(GL_FRONT);

		RenderScene(&depthCubeShader);

		glCullFace(GL_BACK); // don't forget to reset original culling face

		//Second Pass: Normal Drawing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		//Draw Lights
		RenderLights();


		//Set information for the shadow shader
		shadowShader.SetFloat3("viewPos", camera->Position());
		shadowShader.SetMatrix4("view", camera->ViewMat());
		shadowShader.SetMatrix4("projection", camera->ProjMat());

		//Draw the Scene with the shadow shader
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		RenderScene(&shadowShader);
		

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();

		ProcessInput();
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

unsigned int cubeVAO;
unsigned int cubeVBO;
void RenderCube()
{
	if (cubeVAO == 0) {
		CreateBuffers(cubeVAO, cubeVBO, cubeVertices, sizeof(cubeVertices) / sizeof(float));
	}

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

unsigned int quadVAO;
unsigned int quadVBO;
void RenderQuad()
{
	if (quadVAO == 0) {
		CreateBuffers(quadVAO, quadVBO, quadVerts, sizeof(quadVerts) / sizeof(float));
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderScene(Shader* shader)
{
	glm::mat4 model = glm::mat4(1.0);

	//Draw Cubes
	ResourceManager::GetTexture("crateDiffuse").Bind(GL_TEXTURE0);
	for (unsigned int i = 0; i < 10; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		shader->SetMatrix4("model", model);
		RenderCube();
	}

	//move floor plane
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.25f, 0.0f, -12.5f));
	model = glm::scale(model, glm::vec3(25.0f, 1.0f, 25.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader->SetMatrix4("model", model);

	//Draw Floor
	ResourceManager::GetTexture("floorDiffuse").Bind(GL_TEXTURE0);
	RenderQuad();
}


Shader* lightShader;
void RenderLights() {
	if (lightShader == nullptr) {
		lightShader = new Shader("Res/Shaders/LightShader.vert", "Res/Shaders/LightShader.frag");
	}

	glm::mat4 model = glm::mat4(1.0);

	lightShader->SetMatrix4("view", camera->ViewMat());
	lightShader->SetMatrix4("projection", camera->ProjMat());
	//for (int i = 0; i < 4; i++) {
	//	model = glm::mat4(1.0);
	//	model = glm::translate(model, pointLights[i].position);
	//	model = glm::scale(model, glm::vec3(0.2));

	//	lightShader->SetMatrix4("model", model);
	//	lightShader->SetFloat3("lightColor", pointLights[i].diffuse);
	//	RenderCube();
	//}

	//Draw Light
	model = glm::mat4(1.0);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2));
	lightShader->SetMatrix4("model", model);
	lightShader->SetFloat3("lightColor", pointLights[0].diffuse);
	RenderCube();
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