#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "ErrorHandling.h"

#include "Shader.h"


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
	float position[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
	};

	float position2[] = {
		-0.9f, -0.9f,
		-0.66f, -0.9f,
		-0.66f, -0.66f,
		-0.9f, -0.66f
	};

	//Vertex Indices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
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
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	GLCall(glGenVertexArrays(2, &vao[1]));
	glBindVertexArray(vao[1]);
	
	GLCall(glGenBuffers(1, &vbo[1]));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(position2), position2, GL_STATIC_DRAW));


	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	GLCall(glGenBuffers(1, &ibo[1]));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));


	//Create our shader and use the program
	//unsigned int shader = CreateShader(ParseShader("Res/Shaders/Basic.shader"));

	Shader* shader = new Shader("Res/Shaders/Basic.shader");
	shader->BindShaderProgram();


	GLCall(glUseProgram(0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.01f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(_pWindow))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		if (r > 1.0f) {
			increment = -0.01f;
		}
		else if(r < 0.0f) {
			increment = 0.01f;
		}

		r += increment;
		
		shader->BindShaderProgram();
		shader->SetFloat4("u_Color", r, 0.3f, 0.8f, 1.0f);


		GLCall(glBindVertexArray(vao[0]));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		
		
		shader->BindShaderProgram();
		shader->SetFloat4("u_Color", 0.2f, r, 0.8f, 1.0f);

		GLCall(glBindVertexArray(vao[1]));
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