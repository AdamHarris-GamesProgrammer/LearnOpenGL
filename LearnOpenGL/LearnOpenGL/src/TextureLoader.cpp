#include "TextureLoader.h"

unsigned int TextureLoader::LoadTexture(const std::string& filename, bool shouldFlip /*= false*/)
{


	stbi_set_flip_vertically_on_load(shouldFlip);

	int _width, _height, _nrChannels;
	stbi_uc* data = stbi_load(filename.c_str(), &_width, &_height, &_nrChannels, 0);

	if (!data) {
		std::cout << "[ERROR]: Failed to load texture at filepath : " << filename << std::endl;
	}
	ASSERT(data);

	unsigned int texture;

	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLenum format;
	if (_nrChannels == 1) format = GL_RED;
	else if (_nrChannels == 3) format = GL_RGB;
	else if (_nrChannels == 4) {
		format = GL_RGBA;
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}

unsigned int TextureLoader::LoadCubemap(std::vector<std::string> faces, bool shouldFlip /*= false*/)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(shouldFlip);

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "[TEXTURE ERROR]: Could not load cube map at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
