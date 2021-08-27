#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "ErrorHandling.h"

#include "Shader.h"


#include "stb_image.h"



GLFWwindow* _pWindow = nullptr;


void Initialize()
{
	glewInit();

	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	_pWindow = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
	if (!_pWindow)
	{
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(_pWindow);

	//Synchs to monitor refresh rate
	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "[ERROR]: With setting up GLEW" << std::endl;
	}
}

int main(void)
{
	Initialize();



	//Vertex Positions
	//Position, Color, UV Coordinate
	float position[] = {
		// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	float position2[] = {
		-0.66f, -0.66f, 0.0f,	 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, //top right
		-0.66f, -0.9f,  0.0f,	 1.0f, 0.0f, 0.0f,		1.0f, 0.0f, //bottom right
		-0.9f, -0.9f,	0.0f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, //Bottom left
		-0.9f, -0.66f,  0.0f,	 1.0f, 0.0f, 0.0f,		0.0f, 1.0f //top left
	};

	//Vertex Indices
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int vao[2];
	unsigned int vbo[2];
	unsigned int ibo[2];

	GLCall(glGenVertexArrays(1, vao));
	GLCall(glBindVertexArray(vao[0]));

	GLCall(glGenBuffers(1, &vbo[0]));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &ibo[0]));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));

	GLCall(glGenVertexArrays(1, &vao[1]));
	GLCall(glBindVertexArray(vao[1]));

	GLCall(glGenBuffers(1, &vbo[1]));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(position2), position2, GL_STATIC_DRAW));


	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));

	GLCall(glGenBuffers(1, &ibo[1]));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));


	int width, height, nrChannels;
	const char* filename = "Res/Textures/wall.jpg";

	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	unsigned int texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data) {
		//Type of texture, mipmap level, format, with, height, always 0 (legacy stuff), format and datatype of the source image, actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[ERROR]: Failed to load texture at filepath: " << filename << std::endl;
	}

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Res/Textures/awesomeface.png", &width, &height, &nrChannels, 0);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[ERROR]: Failed to load texture at filepath: " << "smiley face" << std::endl;
	}

	//Frees up the memory
	stbi_image_free(data);

	Shader* shader = new Shader("Res/Shaders/Basic.shader");
	shader->BindShaderProgram();


	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.01f;

	shader->SetInt("texture1", 0);
	shader->SetInt("texture2", 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		if (r > 1.0f) {
			increment = -0.01f;
		}
		else if (r < 0.0f) {
			increment = 0.01f;
		}

		r += increment;

		
		//shader->SetFloat4("u_Color", r, 0.3f, 0.8f, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader->BindShaderProgram();
		GLCall(glBindVertexArray(vao[0]));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


		//shader->SetFloat4("u_Color", 0.2f, r, 0.8f, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		GLCall(glBindVertexArray(vao[1]));
		shader->BindShaderProgram();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}

	shader->CleanupShader();

	glfwTerminate();
	return 0;
}