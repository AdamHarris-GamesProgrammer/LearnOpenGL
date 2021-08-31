#include "Shader.h"

Shader::Shader(const std::string& filepath) {
	CreateShader(ParseShader(filepath));
}

void Shader::BindShaderProgram() {
	GLCall(glUseProgram(_shaderProgram));
}

void Shader::CleanupShader() {
	GLCall(glDeleteProgram(_shaderProgram));
}

void Shader::SetFloat(const char* propName, float value) {
	BindShaderProgram();
	GLCall(glUniform1f(GetLocation(propName), value));
}

void Shader::SetInt(const char* propName, int value) {
	BindShaderProgram();
	GLCall(glUniform1i(GetLocation(propName), value));
}

void Shader::SetFloat4(const char* propName, float r, float g, float b, float a) {
	BindShaderProgram();
	GLCall(glUniform4f(GetLocation(propName), r, g, b, a));
}

void Shader::SetMatrix4(const char* propName, float* value)
{
	BindShaderProgram();
	GLCall(glUniformMatrix4fv(GetLocation(propName), 1, GL_FALSE, value));
}

void Shader::SetFloat3(const char* propName, float r, float g, float b)
{
	BindShaderProgram();
	GLCall(glUniform3f(GetLocation(propName), r,g,b));
}

int Shader::GetLocation(const char* propName) {
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

ShaderSource Shader::ParseShader(const std::string& filepath) {
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

unsigned int Shader::CompileShader(const std::string& shader, unsigned int type) {
	GLCall(unsigned int  id = glCreateShader(type));

	const char* src = shader.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "[ERROR]: Failed to compile a shader" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

void Shader::CreateShader(ShaderSource source) {
	GLCall(_shaderProgram = glCreateProgram());

	unsigned int vs = CompileShader(source.vertexSource, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(source.fragmentSource, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(_shaderProgram, vs));
	GLCall(glAttachShader(_shaderProgram, fs));
	GLCall(glLinkProgram(_shaderProgram));
	GLCall(glValidateProgram(_shaderProgram));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}
