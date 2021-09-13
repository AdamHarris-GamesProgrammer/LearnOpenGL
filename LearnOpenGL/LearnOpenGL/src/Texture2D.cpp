#include "Texture2D.h"

Texture2D::Texture2D()
	: _width(0), _height(0), _internalFormat(GL_RGB), _imageFormat(GL_RGB), _wrapS(GL_REPEAT), _wrapT(GL_REPEAT), _filterMin(GL_LINEAR), _filterMax(GL_LINEAR)
{
	glGenTextures(1, &_ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	_width = width;
	_height = height;

	glBindTexture(GL_TEXTURE_2D, _ID);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _imageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMax);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture2D::Bind(GLenum slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, _ID);
}
