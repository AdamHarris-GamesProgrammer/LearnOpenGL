#pragma once
#include "stb_image.h"
#include "GL/glew.h"

#include "ErrorHandling.h"

#include<string>
#include <vector>


static class TextureLoader
{
public:
	const void* LoadTexture(const std::string& filename, bool shouldFlip = false);

	//const void* LoadCubemap(std::vector<std::string> faces, bool shouldFlip = false);
};

