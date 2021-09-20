#pragma once
#include <string>
#include "ResourceManager.h"
#include "Texture2D.h"
#include <vector>
#include "ErrorHandling.h"

class Text
{
public:
	Text();
	~Text();
	Text(std::string text, std::string fontName, glm::vec3 color = glm::vec3(1.0));

	void SetText(std::string text);
	void SetPosition(glm::vec2 pos);
	void SetPosition(float x, float y);
	void SetScale(float s);
	
	std::string text;
	glm::vec3 color;


	std::map<char, Character> _characters;
	std::vector<unsigned int> _VBOs;
	std::vector<unsigned int> _VAOs;

private:
	void UpdateVBO();

	glm::vec2 position;
	float scale;

};

