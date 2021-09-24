#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "ResourceManager.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
	_textShader = ResourceManager::LoadShader("Res/Shaders/Text.vert", "Res/Shaders/Text.frag", nullptr, "text");
	_textShader.SetMatrix4("projection", glm::ortho(0.0f, (float)width, (float)height, 0.0f));
	_textShader.SetInt("text", 0);
}

void TextRenderer::SetFont(std::string fontName)
{
	_characters = ResourceManager::GetFont("generalFont");
}

void TextRenderer::RenderText(Text _text)
{
	glActiveTexture(GL_TEXTURE0);
	_textShader.SetFloat3("textColor", _text.color);

	for (unsigned int i = 0; i < _text.GetLength(); i++) {
		glBindVertexArray(_text._VAOs[i]);
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _text._VBOs[i]));
		glBindTexture(GL_TEXTURE_2D, _characters[_text.modifiedString[i]].textureID);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
}
