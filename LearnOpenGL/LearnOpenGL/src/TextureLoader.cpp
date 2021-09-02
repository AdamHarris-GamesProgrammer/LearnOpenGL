#include "TextureLoader.h"

unsigned int TextureLoader::LoadTexture(const std::string& filename, bool shouldFlip /*= false*/)
{
	unsigned int texture;

	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_set_flip_vertically_on_load(shouldFlip);

	int _width, _height, _nrChannels;
	stbi_uc* data = stbi_load(filename.c_str(), &_width, &_height, &_nrChannels, 0);

	if (!data) {
		std::cout << "[ERROR]: Failed to load texture at filepath : " << filename << std::endl;
	}
	ASSERT(data);

	GLenum format;
	if (_nrChannels == 1) format = GL_RED;
	else if (_nrChannels == 3) format = GL_RGB;
	else if (_nrChannels == 4) format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}
