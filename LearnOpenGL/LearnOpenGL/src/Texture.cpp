#include "Texture.h"

Texture::Texture(const std::string& filename, bool shouldFlip)
{
	GLCall(glGenTextures(1, &_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_set_flip_vertically_on_load(shouldFlip);
	stbi_uc* data = LoadTexture(filename.c_str());

	if (filename.find(".jpg") != std::string::npos) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (filename.find(".png") != std::string::npos) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::BindTexture()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
}

stbi_uc* Texture::LoadTexture(const char* filename)
{
	stbi_uc* data = stbi_load(filename, &_width, &_height, &_nrChannels, 0);

	if (!data) {
		std::cout << "[ERROR]: Failed to load texture at filepath : " << filename << std::endl;
	}
	ASSERT(data);

	return data;
}
