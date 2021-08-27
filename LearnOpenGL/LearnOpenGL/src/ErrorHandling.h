#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

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