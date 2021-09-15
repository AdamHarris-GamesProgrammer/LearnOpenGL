#pragma once

#include <iostream>
#include <GL/glew.h>

#include "ErrorHandling.h"

class Texture2D
{
public:
	unsigned int _ID;
	unsigned int _width, _height;

	unsigned int _internalFormat;
	unsigned int _imageFormat;

	unsigned int _wrapS;
	unsigned int _wrapT;
	unsigned int _filterMin;
	unsigned int _filterMax;

	Texture2D();

	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	void Bind() const;
	void Bind(GLenum slot) const;
};

