#pragma once
#include "stb_image.h"
#include "GL/glew.h"

#include "ErrorHandling.h"
#include<string>


class TextureLoader
{
public:
	unsigned int LoadTexture(const std::string& filename, bool shouldFlip = false);

private:


private:
	int _width;
	int _height; 
	int _nrChannels;

};

