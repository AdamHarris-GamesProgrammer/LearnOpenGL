#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "ResourceManager.h"

TextRenderer::TextRenderer() {}

void TextRenderer::RenderText(Text _text)
{
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetShader("text").SetFloat3("textColor", _text.color);

	for (unsigned int i = 0; i < _text.GetLength(); i++) {
		glBindVertexArray(_text._VAOs[i]);
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _text._VBOs[i]));
		glBindTexture(GL_TEXTURE_2D, _text._characters[_text.modifiedString[i]].textureID);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
}
