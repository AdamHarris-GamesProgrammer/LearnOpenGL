#pragma once
#include "stb_image.h"
#include "GL/glew.h"

#include "ErrorHandling.h"
#include<string>


static class TextureLoader
{
public:
	unsigned int LoadTexture(const std::string& filename, bool shouldFlip = false);
};

