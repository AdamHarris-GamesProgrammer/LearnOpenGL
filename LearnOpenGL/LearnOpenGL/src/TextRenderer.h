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
	Shader _textShader;

	TextRenderer();

	void RenderText(Text _text);
};

