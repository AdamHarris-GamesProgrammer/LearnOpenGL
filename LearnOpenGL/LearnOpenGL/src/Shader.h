#pragma once
#include <gl/glew.h>

#include<iostream>
#include<unordered_map>


#include <string>
#include <sstream>
#include <fstream>

#include "ErrorHandling.h"

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);

	void BindShaderProgram();

	void CleanupShader();

	void SetFloat(const char* propName, float value);

	void SetInt(const char* propName, int value);

	void SetFloat4(const char* propName, float r, float g, float b, float a);

private:
	int GetLocation(const char* propName);

	ShaderSource ParseShader(const std::string& filepath);

	unsigned int CompileShader(const std::string& shader, unsigned int type);

	void CreateShader(ShaderSource source);

private:
	unsigned int _shaderProgram;

	std::unordered_map<const char*, unsigned int>* _propertyMap = new std::unordered_map<const char*, unsigned int>();
};