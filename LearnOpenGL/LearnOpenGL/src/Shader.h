#pragma once
#include <gl/glew.h>

#include<iostream>
#include<unordered_map>


#include <string>
#include <sstream>
#include <fstream>

#include "ErrorHandling.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "GraphicsStructs.h"

class Shader {
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gePath);

	void BindShaderProgram();

	void CleanupShader();

	void SetFloat(const char* propName, float value);

	void SetInt(const char* propName, int value);

	void SetFloat4(const char* propName, float r, float g, float b, float a);

	void SetMatrix4(const char* propName, float* value);
	void SetMatrix4(const char* propName, glm::mat4 value);

	void SetFloat3(const char* propName, float r, float g, float b);
	void SetFloat3(const char* propName, float* values);
	void SetFloat3(const char* propName, glm::vec3 values);

	void SetDirectionalLight(std::string propName, DirectionalLight light);
	void SetPointLight(std::string propName, PointLight light);

	unsigned int GetID() { return _shaderProgram; }

private:
	int GetLocation(const char* propName);

	unsigned int CompileShader(const std::string& shader, unsigned int type);

	void CreateShader(unsigned int vs, unsigned int fs);
	void CreateShader(unsigned int vs, unsigned int fs, unsigned int gs);

	std::stringstream ParseShader(const std::string& path);

private:
	unsigned int _shaderProgram;
public:
	void SetFloat2(const char* param1, glm::vec2 translations);
};