#pragma once

#include <map>
#include <gl/glew.h>
#include <glm.hpp>

#include "Texture2D.h"
#include "Shader.h"


struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class TextRenderer
{
public:
	std::map<char, Character> _characters;

	Shader _textShader;

	TextRenderer(unsigned int width, unsigned int height);

	void Load(std::string font, unsigned int fontSize);

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0));

private:
	unsigned int VAO, VBO;
};

