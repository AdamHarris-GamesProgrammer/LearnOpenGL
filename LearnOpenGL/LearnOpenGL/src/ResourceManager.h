#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANGER_H

#include <map>
#include <string>
#include <GL/glew.h>

#include "Texture2D.h"
#include "Shader.h"
#include "GraphicsStructs.h"

struct Character {
	unsigned int textureID;
	glm::ivec2 _size;
	glm::ivec2 bearing;
	unsigned int advance;
};


class ResourceManager
{
public:
	static std::map<std::string, Shader> _shaders;
	static std::map<std::string, Texture2D> _textures;
	static std::map<std::string, std::map<char, Character>> _fonts;

	static void Init();
	static Shader LoadShader(const char* vs, const char* fs, const char* gs, std::string name);
	static Shader LoadShader(const char* vs, const char* fs, std::string name);
	static Shader& GetShader(std::string name);

	static Texture2D LoadTexture(const char* file, std::string name);
	static Texture2D& GetTexture(std::string name);

	static std::map<char, Character>& LoadFont(const char* font, int _size, std::string name);
	static std::map<char, Character>& GetFont(std::string name);

	static void Clear();

private:
	ResourceManager() {}

	static Shader LoadShaderFromFile(const char* vs, const char* fs, const char* gs = nullptr);
	static Texture2D LoadTextureFromFile(const char* file);
};

#endif // !RESOURCE_MANAGER_H




