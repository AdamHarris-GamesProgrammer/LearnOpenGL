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

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

std::map<char, Character> Characters;


GLFWwindow* _pWindow = nullptr;

unsigned int textVAO, textVBO;

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

unsigned int crateDiffuse;
unsigned int crateSpecular;
unsigned int floorDiffuse;
unsigned int floorSpecular;

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void RenderCube();
void RenderQuad();
void RenderScene(Shader* shader);
void RenderSkybox();
void RenderLights();
void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color);



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

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	TextureLoader textureLoader;
	crateDiffuse = textureLoader.LoadTexture("Res/Textures/container2.png");
	crateSpecular = textureLoader.LoadTexture("Res/Textures/container2_specular.png");
	floorDiffuse = textureLoader.LoadTexture("Res/Textures/floor.png");
	floorSpecular = textureLoader.LoadTexture("Res/Textures/floorSpecular.png");


	Shader* advancedLightingShader = new Shader("Res/Shaders/AdvancedLighting.vert", "Res/Shaders/AdvancedLighting.frag");
	Shader* depthShader = new Shader("Res/Shaders/RenderToDepthMap.vert", "Res/Shaders/RenderToDepthMap.frag");

	Shader* shadowShader = new Shader("Res/Shaders/ShadowMapping.vert", "Res/Shaders/ShadowMapping.frag");

	Shader* depthCubeShader = new Shader("Res/Shaders/DepthCube.vert", "Res/Shaders/DepthCube.frag", "Res/Shaders/DepthCube.geom");

	Shader* textShader = new Shader("Res/Shaders/Text.vert", "Res/Shaders/Text.frag");

	camera = new Camera(_pWindow, width, height, 45.0f);

	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightAmbient = glm::vec3(0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	DirectionalLight sun(glm::vec3(-0.2f, -1.0f, -0.3f), lightAmbient, lightDiffuse, lightSpecular);


	pointLights.push_back(PointLight(glm::vec3(0.2f, 0.2f, 2.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(2.3f, -3.3f, -4.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));
	pointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, -3.0f), lightAmbient, lightDiffuse, lightSpecular, 1.0f, 0.09f, 0.032f));


	for (int i = 0; i < 4; i++) {
		advancedLightingShader->SetFloat3(("lightPosition[" + std::to_string(i) + "]").c_str(), pointLights[i].position);
		advancedLightingShader->SetFloat3(("lightColors[" + std::to_string(i) + "]").c_str(), pointLights[i].diffuse);
	}

	glEnable(GL_DEPTH_TEST);

		//Calculate data for the shadow rendering
	float near_plane = 1.0f, far_plane = 37.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	depthShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

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



	shadowShader->SetFloat3("lightPos", lightPos);
	shadowShader->SetFloat("far_plane", far_plane);
	shadowShader->SetInt("diffuseTexture", 0);
	shadowShader->SetInt("depthMap", 1);
	shadowShader->SetInt("shadows", 1);


	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		__debugbreak();
	}

	FT_Face face;
	if (FT_New_Face(ft, "Res/Fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		__debugbreak();
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Characters.insert(std::pair<char, Character>(c, character));
	}


	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	textShader->SetMatrix4("projection", projection);

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
			depthCubeShader->SetMatrix4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
			depthCubeShader->SetFloat("far_plane", far_plane);
			depthCubeShader->SetFloat3("lightPos", lightPos);
		}


		glCullFace(GL_FRONT);

		RenderScene(depthCubeShader);

		glCullFace(GL_BACK); // don't forget to reset original culling face

		//Second Pass: Normal Drawing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		//Draw Skybox and Lights
		RenderSkybox();
		RenderLights();




		//Set information for the shadow shader
		shadowShader->SetFloat3("viewPos", camera->Position());
		shadowShader->SetMatrix4("view", camera->ViewMat());
		shadowShader->SetMatrix4("projection", camera->ProjMat());



		//Draw the Scene with the shadow shader
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		RenderScene(shadowShader);


		

		RenderText(textShader, "Hello World", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();

		ProcessInput();
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

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
	BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, crateDiffuse);
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
	BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, floorDiffuse);
	RenderQuad();
}


unsigned int skyboxVAO;
unsigned int skyboxVBO;
Shader* skyboxShader;
unsigned int skyboxTexture;
void RenderSkybox() {
	if (skyboxVAO == 0) {
		glGenVertexArrays(1, &skyboxVAO);
		glBindVertexArray(skyboxVAO);

		glGenBuffers(1, &skyboxVBO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		skyboxShader = new Shader("Res/Shaders/Skybox.vert", "Res/Shaders/Skybox.frag");

		std::vector<std::string> faces{
			"Res/Textures/skybox/right.jpg",
			"Res/Textures/skybox/left.jpg",
			"Res/Textures/skybox/top.jpg",
			"Res/Textures/skybox/bottom.jpg",
			"Res/Textures/skybox/front.jpg",
			"Res/Textures/skybox/back.jpg",
		};

		TextureLoader textureLoader;
		skyboxTexture = textureLoader.LoadCubemap(faces);
	}

	glDepthMask(false);
	glBindVertexArray(skyboxVAO);
	glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->ViewMat()));
	skyboxShader->SetMatrix4("projection", camera->ProjMat());
	skyboxShader->SetMatrix4("view", skyboxView);
	BindTexture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(true);
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

void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	s->BindShaderProgram();
	s->SetFloat3("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

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