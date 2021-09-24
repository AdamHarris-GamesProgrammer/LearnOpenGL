#include "Text.h"

Text::~Text()
{

}

Text::Text(std::string _text, std::string fontName, glm::vec3 color /*= glm::vec3(1.0)*/)
	: color(color), position(0.0f), scale(1.0f), lineSpacing(5), alignment(ALIGN_LEFT)
{
	_characters = ResourceManager::GetFont(fontName);
	SetText(_text);
}

Text::Text()
{
}

void Text::SetText(std::string _text)
{
	if (_text == this->_text) return;

	this->_text = _text;
}

void Text::SetPosition(glm::vec2 pos)
{
	position = pos;
}

void Text::SetPosition(float x, float y)
{
	position = glm::vec2(x, y);
}

void Text::SetScale(float s)
{
	scale = s;
}

void Text::SetLineSpacing(int s)
{
	lineSpacing = s;
}

void Text::SetAlignment(Alignment align)
{
	alignment = align;
}

void Text::Finalize()
{
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
	for (c = _text.begin(); c != _text.end(); c++) {
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

	float currentLongest = 0;

	for (c = _text.begin(); c != _text.end(); c++) {
		Character ch = _characters[*c];
		if (*c == '\n') {
			size.y += (_characters['H'].bearing.y * scale) + lineSpacing;
			size.x = 0.0f;
			continue;
		}
		else if (*c == '\t') {
			size.x += (_characters['H'].bearing.x * scale) * 4;
			continue;
		}

		size.x += (ch.advance >> 6) * scale;

		if (size.x > currentLongest) currentLongest = size.x;
	}

	size.x = currentLongest;
	return size;
}

glm::vec2 Text::CalculateLineSize(std::string line)
{
	glm::vec2 size = glm::vec2(0.0f);

	std::string::const_iterator c;
	for (c = line.begin(); c != line.end(); c++) {
		Character ch = _characters[*c];

		if (*c == '\n') break;

		size.x += (ch.advance >> 6) * scale;
	}

	size.y = _characters['H'].bearing.y;

	return size;
}

void Text::UpdateVBO()
{
	float x;
	float y;

	float startingX;
	float startingY;

	_size = CalculateTextBlockSize();

	size_t posOfSlash = _text.find_first_of('\n');

	switch (alignment) {
	case ALIGN_LEFT:
		startingX = position.x;
		startingY = position.y;
		break;
	case ALIGN_CENTER:
		startingX = position.x - (_size.x / 4);
		startingY = position.y - (_size.y / 4);

		
		if (posOfSlash != std::string::npos) {
			std::string newStr = _text.substr(0, posOfSlash - 1);
			glm::vec2 sizeOfLine = CalculateLineSize(newStr);
			startingX = position.x - (sizeOfLine.x / 2);
		}
		break;
	case ALIGN_RIGHT:
		startingX = position.x - (_size.x / 4);
		startingY = position.y - (_size.y / 4);

		
		if (posOfSlash != std::string::npos) {
			std::string newStr = _text.substr(0, posOfSlash);
			glm::vec2 sizeOfLine = CalculateLineSize(newStr);
			startingX = (position.x + (_size.x - sizeOfLine.x));
		}
		else {
			glm::vec2 sizeOfLine = CalculateLineSize(_text);
			startingX = (position.x + (_size.x - sizeOfLine.x));
		}
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
	size_t pos = 0;
	for (c = _text.begin(); c != _text.end(); c++) {
		Character ch = _characters[*c];

		if (*c == '\n') {
			y += (_characters['H'].bearing.y * scale) + lineSpacing; 
			x = startingX;

			if (alignment == ALIGN_CENTER) {
				size_t posOfSlash = _text.find_first_of('\n', pos + 1);

				std::string newStr = _text.substr(pos + 1);
				glm::vec2 sizeOfLine = CalculateLineSize(newStr);
				x = position.x - (sizeOfLine.x / 2);
			}

			if (alignment == ALIGN_RIGHT) {
				size_t posOfSlash = _text.find_first_of('\n', pos + 1);
				if (posOfSlash != std::string::npos) {
					std::string newStr = _text.substr(0, posOfSlash - 1);
					glm::vec2 sizeOfLine = CalculateLineSize(newStr);
					startingX = (position.x + _size.x - sizeOfLine.x);
				}
				else {
					glm::vec2 sizeOfLine = CalculateLineSize(_text);
					startingX = (position.x + _size.x - sizeOfLine.x);
				}
			}

			continue;
		}
		else if (*c == '\t') {
			x += (_characters['H'].bearing.x * scale) * 4;
			continue;
		}

		float xpos = x + ch.bearing.x * scale;
		float ypos = y + (_characters['H'].bearing.y - ch.bearing.y) * scale;

		float w = ch._size.x * scale;
		float h = ch._size.y * scale;

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
		pos++;
	}

	CalculateLength();

	//glBindVertexArray(0);
}


