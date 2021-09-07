#pragma once
#include "stb_image.h"
#include "GL/glew.h"

#include "ErrorHandling.h"

#include<string>
#include <vector>


static class TextureLoader
{
public:
	unsigned int LoadTexture(const std::string& filename, bool shouldFlip = false);

	unsigned int LoadCubemap(std::vector<std::string> faces, bool shouldFlip = false);
};

