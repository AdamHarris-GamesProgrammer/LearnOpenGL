#pragma once
#include<iostream>
#include<unordered_map>
#include <gl/glew.h>
#include "ErrorHandling.h"
#include <string>
#include <sstream>
#include <fstream>

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath) {
		CreateShader(ParseShader(filepath));
	}

	void BindShaderProgram() {
		GLCall(glUseProgram(_shaderProgram));
	}

	void CleanupShader() {
		GLCall(glDeleteProgram(_shaderProgram));
	}

	void SetFloat(const char* propName, float value) {
		GLCall(glUniform1f(GetLocation(propName), value));
	}

	void SetInt(const char* propName, int value) {
		GLCall(glUniform1i(GetLocation(propName), value));
	}

	void SetFloat4(const char* propName, float r, float g, float b, float a) {
		GLCall(glUniform4f(GetLocation(propName), r, g, b, a));
	}

private:
	int GetLocation(const char* propName) {
		int location = -1;

		std::unordered_map<const char*, unsigned int>::const_iterator it = _propertyMap->find(propName);

		if (it != _propertyMap->end()) {
			location = it->second;
		}
		else {
			GLCall(location = glGetUniformLocation(_shaderProgram, propName));
			ASSERT(location != -1);
			_propertyMap->emplace(propName, location);
		}

		return location;
	}

	ShaderSource ParseShader(const std::string& filepath) {
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

	unsigned int CompileShader(const std::string& shader, unsigned int type) {
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

	void CreateShader(ShaderSource source) {
		_shaderProgram = glCreateProgram();

		unsigned int vs = CompileShader(source.vertexSource, GL_VERTEX_SHADER);
		unsigned int fs = CompileShader(source.fragmentSource, GL_FRAGMENT_SHADER);

		glAttachShader(_shaderProgram, vs);
		glAttachShader(_shaderProgram, fs);
		glLinkProgram(_shaderProgram);
		glValidateProgram(_shaderProgram);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

private:
	unsigned int _shaderProgram;

	std::unordered_map<const char*, unsigned int>* _propertyMap = new std::unordered_map<const char*, unsigned int>();
};