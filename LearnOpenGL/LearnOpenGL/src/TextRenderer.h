#pragma once

#include <map>
#include <gl/glew.h>
#include <glm.hpp>

#include "Texture2D.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Text.h"




class TextRenderer
{
public:
	std::map<char, Character> _characters;

	Shader _textShader;

	TextRenderer(unsigned int width, unsigned int height);

	void SetFont(std::string font);

	void RenderText(Text _text);

private:
};

