#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "ResourceManager.h"
#include "Texture2D.h"
#include "Text.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

class Button
{
public:
	Button();
	~Button();
	Button(glm::vec2 pos, glm::vec2 size = glm::vec2(160.0f, 80.0f));

	Text text;
	bool IsPressed(glm::vec2 cursorPosition);

	void Draw(SpriteRenderer* spriteRend, TextRenderer* textRend);

private:
	glm::vec2 _position;
	glm::vec2 _size;
	Texture2D _texture;
};

