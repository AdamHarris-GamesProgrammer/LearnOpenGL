#pragma once
#include <string>
#include "ResourceManager.h"
#include "Texture2D.h"
#include <vector>
#include "ErrorHandling.h"
#include "UIObject.h"

class Text : public UIObject
{
public:
	Text();
	~Text();
	Text(std::string _text, std::string fontName = "generalFont", glm::vec3 color = glm::vec3(1.0));

	void SetText(std::string _text);
	void SetLineSpacing(int s);
	void Finalize();

	int GetLength();
	
	std::string modifiedString;

	glm::vec3 color;

	std::map<char, Character> _characters;
	std::vector<unsigned int> _VBOs;
	std::vector<unsigned int> _VAOs;
private:
	std::string _text;

	void UpdateVBO();
	void CalculateLength();

	glm::vec2 CalculateTextBlockSize();
	glm::vec2 CalculateLineSize(std::string line);

	int calcedLength;
	int lineSpacing;
};

