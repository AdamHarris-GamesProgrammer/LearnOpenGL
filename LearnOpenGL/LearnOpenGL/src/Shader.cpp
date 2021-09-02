#include "Shader.h"

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	std::ifstream stream(vsPath);

	std::string line;
	std::stringstream vs;

	while (getline(stream, line)) {
		vs << line << '\n';
	}

	unsigned int v =CompileShader(vs.str(), GL_VERTEX_SHADER);

	std::ifstream fsStream(fsPath);

	std::stringstream fs;

	while (getline(fsStream, line))
	{
		fs << line << '\n';
	}

	unsigned int f = CompileShader(fs.str(), GL_FRAGMENT_SHADER);

	CreateShader(v, f);
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

void Shader::SetFloat3(const char* propName, float* values)
{
	BindShaderProgram();
	GLCall(glUniform3f(GetLocation(propName), values[0], values[1], values[2]));
}

void Shader::SetFloat3(const char* propName, glm::vec3 values)
{
	BindShaderProgram();
	GLCall(glUniform3f(GetLocation(propName), values.r, values.g, values.b));
	
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

void Shader::CreateShader(unsigned int vs, unsigned int fs)
{
	GLCall(_shaderProgram = glCreateProgram());
	GLCall(glAttachShader(_shaderProgram, vs));
	GLCall(glAttachShader(_shaderProgram, fs));
	GLCall(glLinkProgram(_shaderProgram));
	GLCall(glValidateProgram(_shaderProgram));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

