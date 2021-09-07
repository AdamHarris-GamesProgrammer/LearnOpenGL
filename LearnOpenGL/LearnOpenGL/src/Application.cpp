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

Shader* currentShader = nullptr;

Shader* framebufferShader = nullptr;
Shader* inversionShader = nullptr;
Shader* grayscaleShader = nullptr;
Shader* sharpenShader = nullptr;
Shader* blurShader = nullptr;
Shader* edgeShader = nullptr;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int LoadCubemap(std::vector<std::string> faces);

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

	unsigned int screenQuadVao;
	unsigned int screenQuadVbo;

	GLCall(glGenVertexArrays(1, &screenQuadVao));
	GLCall(glBindVertexArray(screenQuadVao));

	GLCall(glGenBuffers(1, &screenQuadVbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, screenQuadVbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));



	unsigned int pointVAO;
	unsigned int pointVBO;

	GLCall(glGenVertexArrays(1, &pointVAO));
	GLCall(glBindVertexArray(pointVAO));

	GLCall(glGenBuffers(1, &pointVBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, pointVBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW));


	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));

	Shader* geomTester = new Shader("Res/Shaders/Geometry.vert", "Res/Shaders/Geometry.frag", "Res/Shaders/GeometryShader.geom");



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

	unsigned int quadVao;
	unsigned int quadVbo;
	CreateBuffers(quadVao, quadVbo, quadVerts, sizeof(quadVerts) / sizeof(float));

	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	TextureLoader textureLoader;
	unsigned int diffuseTexture = textureLoader.LoadTexture("Res/Textures/container2.png");
	unsigned int specularTexture = textureLoader.LoadTexture("Res/Textures/container2_specular.png");
	unsigned int grassTexture = textureLoader.LoadTexture("Res/Textures/grass.png", true);
	unsigned int windowTexture = textureLoader.LoadTexture("Res/Textures/window.png", true);

	Shader* objectShader = new Shader("Res/Shaders/BasicShader.vert", "Res/Shaders/BasicShader.frag");
	Shader* lightShader = new Shader("Res/Shaders/LightShader.vert", "Res/Shaders/LightShader.frag");
	Shader* vegetationShader = new Shader("Res/Shaders/ModelShader.vert", "Res/Shaders/GrassShader.frag");
	Shader* modelShader = new Shader("Res/Shaders/ModelShader.vert", "Res/Shaders/ModelShader.frag");
	Shader* skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");
	Shader* environmentShader = new Shader("Res/Shaders/EnvironmentMapping.vert", "Res/Shaders/EnvironmentMapping.frag");


	framebufferShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/Framebuffer.frag");
	inversionShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/InversionEffect.frag");
	grayscaleShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/GrayscaleEffect.frag");
	sharpenShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/SharpenKernal.frag");
	blurShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/BlurKernal.frag");
	edgeShader = new Shader("Res/Shaders/Framebuffer.vert", "Res/Shaders/EdgeDetectionKernal.frag");

	currentShader = framebufferShader;

	std::vector<std::string> faces{
		"Res/Textures/skybox/right.jpg",
		"Res/Textures/skybox/left.jpg",
		"Res/Textures/skybox/top.jpg",
		"Res/Textures/skybox/bottom.jpg",
		"Res/Textures/skybox/front.jpg",
		"Res/Textures/skybox/back.jpg",
	};

	unsigned int skybox = LoadCubemap(faces);

	//Model backpack("Res/Models/backpack/backpack.obj");

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	camera = new Camera(_pWindow, width, height, 45.0f);

	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.2f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun(glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);

	std::vector<PointLight> pointLights;
	pointLights.push_back(PointLight(glm::vec3(0.7f, 0.2f, 2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));

	std::vector<glm::vec3> vegetationPositions;
	vegetationPositions.push_back(glm::vec3(1.5f, 0.0f, -1.0f));
	vegetationPositions.push_back(glm::vec3(-3.8f, 0.0f, -2.5f));
	vegetationPositions.push_back(glm::vec3(2.5, 0.0f, 2.3f));
	vegetationPositions.push_back(glm::vec3(-1.2f, 0.0f, 1.5f));

	std::vector<glm::vec3> windowPositions;
	windowPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windowPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windowPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windowPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windowPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	objectShader->SetDirectionalLight("u_dirLight", sun);
	objectShader->SetInt("u_material.diffuse", 0);
	objectShader->SetInt("u_material.specular", 1);
	objectShader->SetFloat("u_material.shininess", 32.0f);

	lightShader->SetFloat3("u_lightColor", glm::value_ptr(lightDiffuse));

	modelShader->SetDirectionalLight("u_dirLight", sun);
	modelShader->SetFloat("u_material.shininess", 32.0f);
	vegetationShader->SetInt("texture1", 0);

	for (GLuint i = 0; i < 4; i++) {
		std::string number = std::to_string(i);
		std::string base = "u_pointLight[" + number + "]";
		objectShader->SetPointLight(base, pointLights[i]);
	}


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Second Pass: Draw the scene normally
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		camera->Update(deltaTime);

		glm::mat4 view = camera->ViewMat();
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)width / (float)height, 0.1f, 100.0f);

		glDepthMask(false);
		glBindVertexArray(skyboxVAO);
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->ViewMat()));
		skyboxShader->SetMatrix4("projection", camera->Proj());
		skyboxShader->SetMatrix4("view", skyboxView);
		BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(true);


		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, diffuseTexture);
		BindTexture(GL_TEXTURE1, GL_TEXTURE_2D, specularTexture);

		objectShader->SetFloat3("u_viewPos", camera->Position());
		objectShader->SetMatrix4("u_view", view);
		objectShader->SetMatrix4("u_projection", camera->Proj());

		//Draw Cubes
		GLCall(glBindVertexArray(cubeVao));

		BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skybox);
		environmentShader->SetMatrix4("view", view);
		environmentShader->SetMatrix4("projection", projection);
		environmentShader->SetFloat3("cameraPos", camera->Position());
		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
			//objectShader->SetMatrix4("u_model", glm::value_ptr(model));
			environmentShader->SetMatrix4("model", model);
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


		modelShader->SetFloat3("u_viewPos", camera->View());

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		modelShader->SetMatrix4("view", view);
		modelShader->SetMatrix4("projection", camera->Proj());
		modelShader->SetMatrix4("model", glm::value_ptr(model));
		//backpack.Draw(modelShader);

		std::map<float, glm::vec3> sorted = std::map<float, glm::vec3>();
		for (unsigned int i = 0; i < windowPositions.size(); i++) {
			float distance = glm::length(camera->GPosition() - windowPositions[i]);
			sorted[distance] = windowPositions[i];
		}

		//Windows
		GLCall(glBindVertexArray(quadVao));

		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, windowTexture);

		vegetationShader->SetMatrix4("view", view);
		vegetationShader->SetMatrix4("projection", camera->Proj());

		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			vegetationShader->SetMatrix4("model", glm::value_ptr(model));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}
		//Grass
		BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, grassTexture);
		for (unsigned int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, vegetationPositions[i]);
			vegetationShader->SetMatrix4("model", glm::value_ptr(model));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}

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

	if (glfwGetKey(_pWindow, GLFW_KEY_1)) currentShader = framebufferShader;
	if (glfwGetKey(_pWindow, GLFW_KEY_2)) currentShader = inversionShader;
	if (glfwGetKey(_pWindow, GLFW_KEY_3)) currentShader = grayscaleShader;
	if (glfwGetKey(_pWindow, GLFW_KEY_4)) currentShader = sharpenShader;
	if (glfwGetKey(_pWindow, GLFW_KEY_5)) currentShader = blurShader;
	if (glfwGetKey(_pWindow, GLFW_KEY_6)) currentShader = edgeShader;
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

unsigned int LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(false);

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "[TEXTURE ERROR]: Could not load cube map at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}