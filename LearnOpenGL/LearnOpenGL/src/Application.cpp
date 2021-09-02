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


GLFWwindow* _pWindow = nullptr;

unsigned int width = 1280;
unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

Camera* camera;

//Vertex Positions
//Position, Normal, UV Coordinate
float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,     0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(4.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float quadVerts[] = {
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
};

glm::vec3 vegetationPositions[] = {
	glm::vec3(1.5f, 0.0f, -1.0f),
	glm::vec3(-3.8f, 0.0f, -2.5f),
	glm::vec3(2.5, 0.0f, 2.3f),
	glm::vec3(-1.2f, 0.0f, 1.5f)
};

glm::vec3 windowPositions[] = {
	glm::vec3(-1.5f,  0.0f, -0.48f),
	glm::vec3(1.5f,  0.0f,  0.51f),
	glm::vec3(0.0f,  0.0f,  0.7f),
	glm::vec3(-0.3f,  0.0f, -2.3f),
	glm::vec3(0.5f,  0.0f, -0.6f)
};

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

	unsigned int cubeVao;
	unsigned int vbo;

	GLCall(glGenVertexArrays(1, &cubeVao));
	GLCall(glBindVertexArray(cubeVao));

	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));

	unsigned int grassVao;
	unsigned int grassVbo;

	GLCall(glGenVertexArrays(1, &grassVao));
	GLCall(glBindVertexArray(grassVao));

	GLCall(glGenBuffers(1, &grassVbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, grassVbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));


	TextureLoader textureLoader;
	unsigned int diffuseTexture = textureLoader.LoadTexture("Res/Textures/container2.png");
	unsigned int specularTexture = textureLoader.LoadTexture("Res/Textures/container2_specular.png");
	unsigned int grassTexture = textureLoader.LoadTexture("Res/Textures/window.png", true);

	Shader* objectShader = new Shader("Res/Shaders/BasicShader.vert", "Res/Shaders/BasicShader.frag");
	Shader* lightShader = new Shader("Res/Shaders/LightShader.vert", "Res/Shaders/LightShader.frag");
	Shader* vegetationShader = new Shader("Res/Shaders/ModelShader.vert", "Res/Shaders/GrassShader.frag");
	Shader* modelShader = new Shader("Res/Shaders/ModelShader.vert", "Res/Shaders/ModelShader.frag");

	//Model backpack("Res/Models/backpack/backpack.obj");

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	camera = new Camera(_pWindow, width, height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.2f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun( glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);

	PointLight pointLights[] = {
		PointLight(glm::vec3(0.7f,  0.2f,  2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f),
		PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f),
		PointLight(glm::vec3(-4.0f,  2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f),
		PointLight(glm::vec3(0.0f,  0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f),
	};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera->Update(deltaTime);

		/* Render here */
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		objectShader->SetDirectionalLight("u_dirLight", sun);
		for (GLuint i = 0; i < 4; i++) {
			std::string number = std::to_string(i);
			std::string base = "u_pointLight[" + number + "]";
			objectShader->SetPointLight(base, pointLights[i]);
		}


		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, diffuseTexture));
		glActiveTexture(GL_TEXTURE1);
		GLCall(glBindTexture(GL_TEXTURE_2D, specularTexture));


		objectShader->SetInt("u_material.diffuse", 0);
		objectShader->SetInt("u_material.specular", 1);
		objectShader->SetFloat("u_material.shininess", 32.0f);
		objectShader->SetFloat3("u_viewPos", camera->Position());
		objectShader->SetMatrix4("u_view", camera->View());
		objectShader->SetMatrix4("u_projection", camera->Proj());

		GLCall(glBindVertexArray(cubeVao));
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
			objectShader->SetMatrix4("u_model", glm::value_ptr(model));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		lightShader->SetFloat3("u_lightColor", glm::value_ptr(lightDiffuse));
		lightShader->SetMatrix4("u_view", camera->View());
		lightShader->SetMatrix4("u_projection", camera->Proj());
		for (int i = 0; i < 4; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLights[i].position);
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			lightShader->SetMatrix4("u_model", glm::value_ptr(model));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}


		modelShader->SetDirectionalLight("u_dirLight", sun);
		modelShader->SetFloat3("u_viewPos", camera->View());
		modelShader->SetFloat("u_material.shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		modelShader->SetMatrix4("view", camera->View());
		modelShader->SetMatrix4("projection", camera->Proj());
		modelShader->SetMatrix4("model", glm::value_ptr(model));
		//backpack.Draw(modelShader);

		glBindVertexArray(grassVao);

		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, grassTexture));

		vegetationShader->SetInt("texture1", 0);
		vegetationShader->SetMatrix4("view", camera->View());
		vegetationShader->SetMatrix4("projection", camera->Proj());
		for (unsigned int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, vegetationPositions[i]);
			vegetationShader->SetMatrix4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
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