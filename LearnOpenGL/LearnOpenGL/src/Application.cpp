#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

GLFWwindow* _pWindow = nullptr;

static void GLClearError() {
	//GL_NO_ERROR = 0
	while (glGetError() != GL_NO_ERROR) {

	}
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL ERROR]: (" << error << "): " << function << " " << file << " Line: " << line << std::endl;
		return false;
	}

	return true;
}

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

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(const std::string& shader, unsigned int type) {
	unsigned int  id = glCreateShader(type);

	const char* src = shader.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "[ERROR]: Failed to compile a shader" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static unsigned int CreateShader(ShaderSource source) {
	return CreateShader(source.vertexSource, source.fragmentSource);
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
		-1.0f, -1.0f,
		-0.66f, -1.0f,
		-0.66f, -0.66f,
		-1.0f, -0.66f
	};

	//Vertex Indices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	//Create Vertex Buffer
	unsigned int vertexBuffer;
	GLCall(glGenBuffers(1, &vertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW));

	//Create Index Buffer
	unsigned int indexBuffer;
	GLCall(glGenBuffers(1, &indexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int vao2;
	GLCall(glGenVertexArrays(2, &vao2));
	glBindVertexArray(vao2);
	
	//Create Vertex Buffer 2
	unsigned int vertexBuffer2;
	GLCall(glGenBuffers(1, &vertexBuffer2));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(position2), position2, GL_STATIC_DRAW));


	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int indexBuffer2;
	GLCall(glGenBuffers(1, &indexBuffer2));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer2));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	

	//Create our shader and use the program
	unsigned int shader = CreateShader(ParseShader("Res/Shaders/Basic.shader"));
	GLCall(glUseProgram(shader));

	//find the uniform and check that it is valid, before assigning a colour to it
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	
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
		
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));


		GLCall(glBindVertexArray(vao2));
		//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		

		GLCall(glBindVertexArray(vao));
		//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(_pWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}


