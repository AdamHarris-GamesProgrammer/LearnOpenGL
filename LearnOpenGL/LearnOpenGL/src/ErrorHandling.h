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
	while (GLenum errorCode = glGetError()) {
		std::string error;

		switch (errorCode) {
		case GL_INVALID_ENUM: error = "INVLAID ENUM"; break;
		case GL_INVALID_OPERATION: error = "INVALID OPERATION"; break;
		case GL_INVALID_VALUE: error = "INVALID VALUE"; break;
		case GL_STACK_OVERFLOW: error = "STACK OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: error = "STACK UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID FRAMEBUFFER OPERATION"; break;
		}

		std::cout << "[OpenGL ERROR]: (" << error << "): " << function << " " << file << " Line: " << line << std::endl;
		return false;
	}

	return true;
}