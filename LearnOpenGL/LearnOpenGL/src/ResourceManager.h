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
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};


class ResourceManager
{
public:
	static std::map<std::string, Shader> _shaders;
	static std::map<std::string, Texture2D> _textures;
	static std::map<std::string, std::map<char, Character>> _fonts;

	static Shader LoadShader(const char* vs, const char* fs, const char* gs, std::string name);
	static Shader& GetShader(std::string name);

	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
	static Texture2D& GetTexture(std::string name);

	static std::map<char, Character>& LoadFont(const char* font, int size, std::string name);
	static std::map<char, Character>& GetFont(std::string name);

	static void Clear();

private:
	ResourceManager() {}

	static Shader LoadShaderFromFile(const char* vs, const char* fs, const char* gs = nullptr);
	static Texture2D LoadTextureFromFile(const char* file, bool alpha);
};

#endif // !RESOURCE_MANAGER_H




