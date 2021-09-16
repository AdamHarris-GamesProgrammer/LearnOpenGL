#include "Shader.h"

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	std::stringstream vs;
	std::stringstream fs;

	vs = ParseShader(vsPath);
	fs = ParseShader(fsPath);

	unsigned int v = CompileShader(vs.str(), GL_VERTEX_SHADER);
	unsigned int f = CompileShader(fs.str(), GL_FRAGMENT_SHADER);

	CreateShader(v, f);
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gePath)
{
	std::stringstream vs;
	std::stringstream fs;
	std::stringstream gs;

	vs = ParseShader(vsPath);
	fs = ParseShader(fsPath);
	gs = ParseShader(gePath);

	unsigned int v = CompileShader(vs.str(), GL_VERTEX_SHADER);
	unsigned int f = CompileShader(fs.str(), GL_FRAGMENT_SHADER);
	unsigned int g = CompileShader(gs.str(), GL_GEOMETRY_SHADER);

	CreateShader(v, f, g);
}

void Shader::BindShaderProgram() {
	GLCall(glUseProgram(_shaderProgram));
}

void Shader::CleanupShader() {
	GLCall(glDeleteProgram(_shaderProgram));
}

void Shader::SetFloat(const char* propName, float value) {
	BindShaderProgram();
	//std::cout << propName << std::endl;
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

void Shader::SetFloat4(const char* propName, glm::vec4 value)
{
	BindShaderProgram();
	GLCall(glUniform4f(GetLocation(propName), value.x, value.y, value.z, value.w));
}

void Shader::SetFloat4(const char* propName, float* value)
{
	BindShaderProgram();
	GLCall(glUniform4f(GetLocation(propName), value[0], value[1], value[2], value[3]));
}

void Shader::SetMatrix4(const char* propName, float* value)
{
	BindShaderProgram();
	GLCall(glUniformMatrix4fv(GetLocation(propName), 1, GL_FALSE, value));
}

void Shader::SetMatrix4(const char* propName, glm::mat4 value)
{
	BindShaderProgram();
	GLCall(glUniformMatrix4fv(GetLocation(propName), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::SetFloat3(const char* propName, float r, float g, float b)
{
	BindShaderProgram();
	//std::cout << propName << std::endl;
	GLCall(glUniform3f(GetLocation(propName), r,g,b));
}

void Shader::SetFloat3(const char* propName, float* values)
{
	BindShaderProgram();
	//std::cout << propName << std::endl;
	GLCall(glUniform3f(GetLocation(propName), values[0], values[1], values[2]));
}

void Shader::SetFloat3(const char* propName, glm::vec3 values)
{
	BindShaderProgram();
	//std::cout << propName << std::endl;
	GLCall(glUniform3f(GetLocation(propName), values.r, values.g, values.b));
	
}

void Shader::SetDirectionalLight(std::string propName, DirectionalLight light)
{
	BindShaderProgram();

	SetFloat3((propName + ".ambient").c_str(), light.ambient);
	SetFloat3((propName + ".diffuse").c_str(), light.diffuse);
	SetFloat3((propName + ".specular").c_str(), light.ambient);
	SetFloat3((propName + ".direction").c_str(), light.direction);
}

void Shader::SetPointLight(std::string propName, PointLight light)
{
	BindShaderProgram();

	SetFloat3((propName + ".position").c_str(), light.position);
	SetFloat3((propName + ".ambient").c_str(), light.ambient);
	SetFloat3((propName + ".diffuse").c_str(), light.diffuse);
	SetFloat3((propName + ".specular").c_str(), light.ambient);
	SetFloat((propName + ".constant").c_str(), light.constant);
	SetFloat((propName + ".linear").c_str(), light.linear);
	SetFloat((propName + ".quadratic").c_str(), light.quadratic);
}

int Shader::GetLocation(const char* propName) {
	int location = -1;
	
	GLCall(location = glGetUniformLocation(_shaderProgram, propName));
	ASSERT(location != -1);

	return location;
}


unsigned int Shader::CompileShader(const std::string& _shader, unsigned int type) {
	GLCall(unsigned int  id = glCreateShader(type));

	const char* src = _shader.c_str();

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

void Shader::CreateShader(unsigned int vs, unsigned int fs, unsigned int gs)
{
	GLCall(_shaderProgram = glCreateProgram());
	GLCall(glAttachShader(_shaderProgram, vs));
	GLCall(glAttachShader(_shaderProgram, fs));
	GLCall(glAttachShader(_shaderProgram, gs));
	GLCall(glLinkProgram(_shaderProgram));
	GLCall(glValidateProgram(_shaderProgram));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	GLCall(glDeleteShader(gs));
}

std::stringstream Shader::ParseShader(const std::string& path)
{
	std::ifstream stream(path);

	std::string line;
	std::stringstream ss;

	if (stream.fail()) {
		std::cout << "[SHADER ERROR]: Cannot open shader at path: " << path << std::endl;
		__debugbreak();
	}

	while (getline(stream, line)) {
		if (line.find("#include ") != std::string::npos) {
			std::string directory = path.substr(0, path.find_last_of('/'));

			size_t lastMark = line.find_last_of("\"");
			size_t firstMark = line.find_first_of("\"");

			size_t count = (lastMark - firstMark) - 1;

			std::string includeFile = line.substr(firstMark + 1, count);

			size_t comment = line.find("//!");

			count = (comment - firstMark) - 2;

			std::string newPath = directory + "/" + line.substr(firstMark + 1, count - 1);

			ss << ParseShader(newPath).str();
		}
		else
		{
			ss << line << '\n';
		}

		line = "";
	}


	return ss;
}

void Shader::SetFloat2(const char* propName, glm::vec2 values)
{
	BindShaderProgram();
	//std::cout << propName << std::endl;
	GLCall(glUniform2f(GetLocation(propName), values.x, values.y));
}

