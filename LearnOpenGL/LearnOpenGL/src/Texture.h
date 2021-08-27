#pragma once
#include "stb_image.h"
#include "GL/glew.h"

#include "ErrorHandling.h"
#include<string>


class Texture
{
public:
	Texture(const std::string& filename, bool shouldFlip = false);

	void BindTexture();

private:
	stbi_uc* LoadTexture(const char* filename);


private:
	unsigned int _textureID;
	int _width;
	int _height; 
	int _nrChannels;

};

