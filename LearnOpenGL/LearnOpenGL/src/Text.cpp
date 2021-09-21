#include "Text.h"

Text::~Text()
{

}

Text::Text(std::string text, std::string fontName, glm::vec3 color /*= glm::vec3(1.0)*/)
	: color(color), position(0.0f), scale(1.0f), lineSpacing(5), alignment(ALIGN_LEFT)
{
	_characters = ResourceManager::GetFont(fontName);
	SetText(text);
	
}

Text::Text()
{
}

void Text::SetText(std::string text)
{
	if (text == this->text) return;

	this->text = text;
	UpdateVBO();
}

void Text::SetPosition(glm::vec2 pos)
{
	position = pos;
	UpdateVBO();
}

void Text::SetPosition(float x, float y)
{
	position = glm::vec2(x, y);
	UpdateVBO();
}

void Text::SetScale(float s)
{
	scale = s;
	UpdateVBO();
}

void Text::SetLineSpacing(int s)
{
	lineSpacing = s;
	UpdateVBO();
}

void Text::SetAlignment(Alignment align)
{
	alignment = align;
	UpdateVBO();
}

int Text::GetLength()
{
	return calcedLength;
}

void Text::CalculateLength()
{
	int i = 0;

	modifiedString = "";

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		if (*c == '\n') {

		}
		else if (*c == '\t') {

		}
		else
		{
			i++;
			modifiedString.push_back(*c);
		}
	}

	calcedLength = i;
}

glm::vec2 Text::CalculateTextBlockSize()
{
	glm::vec2 size = glm::vec2(0.0f);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = _characters[*c];
		if (*c == '\n') {
			size.y += (_characters['H'].bearing.y * scale) + lineSpacing;
			continue;
		}
		else if (*c == '\t') {
			size.x += (_characters['H'].bearing.x * scale) * 4;
			continue;
		}

		float xpos = size.x + ch.bearing.x * scale;
		float ypos = size.y + (_characters['H'].bearing.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		size.x += (ch.advance >> 6) * scale;
	}

	return size;
}

void Text::UpdateVBO()
{
	float x;
	float y;

	float startingX;
	float startingY;

	switch (alignment) {
	case ALIGN_LEFT:
		startingX = position.x;
		startingY = position.y;
		break;
	case ALIGN_CENTER:
		glm::vec2 size = CalculateTextBlockSize();
		startingX = position.x - (size.x / 4);
		startingY = position.y - (size.y / 4);

		break;
	case ALIGN_RIGHT:

		break;
	}

	x = startingX;
	y = startingY;

	for (unsigned int vbo : _VBOs)
		glDeleteBuffers(1, &vbo);

	for (unsigned int vao : _VAOs)
		glDeleteVertexArrays(1, &vao);

	_VBOs.clear();
	_VAOs.clear();
	
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = _characters[*c];

		if (*c == '\n') {
			y += (_characters['H'].bearing.y * scale) + lineSpacing; 
			x = startingX;
			continue;
		}
		else if (*c == '\t') {
			x += (_characters['H'].bearing.x * scale) * 4;
			continue;
		}

		float xpos = x + ch.bearing.x * scale;
		float ypos = y + (_characters['H'].bearing.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};

		unsigned int newVAO;
		glGenVertexArrays(1, &newVAO);
		glBindVertexArray(newVAO);

		unsigned int newVBO;
		glGenBuffers(1, &newVBO);
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, newVBO));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));

		glActiveTexture(GL_TEXTURE0);
		GLCall(glBindTexture(GL_TEXTURE_2D, ch.textureID));

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		_VBOs.push_back(newVBO);
		_VAOs.push_back(newVAO);

		x += (ch.advance >> 6) * scale;
	}

	CalculateLength();

	//glBindVertexArray(0);
}


