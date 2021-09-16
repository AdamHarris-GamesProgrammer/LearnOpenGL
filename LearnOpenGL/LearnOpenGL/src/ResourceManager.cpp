#include "ResourceManager.h"
#include "stb_image.h"

std::map<std::string, Shader> ResourceManager::_shaders;

std::map<std::string, Texture2D> ResourceManager::_textures;

Shader ResourceManager::LoadShader(const char* vs, const char* fs, const char* gs, std::string name)
{
	_shaders[name] = LoadShaderFromFile(vs, fs, gs);
	return _shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return _shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	_textures[name] = LoadTextureFromFile(file, alpha);
	return _textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return _textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : _shaders)
		glDeleteProgram(iter.second.GetID());
	for (auto iter : _textures)
		glDeleteTextures(1, &iter.second._ID);
}

Shader ResourceManager::LoadShaderFromFile(const char* vs, const char* fs, const char* gs /*= nullptr*/)
{
	Shader _shader;
	if (gs != nullptr)
		_shader = Shader(vs, fs, gs);
	else
		_shader = Shader(vs, fs);

	return _shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
	Texture2D _texture;

	if (alpha) {
		_texture._imageFormat = GL_RGBA;
		_texture._internalFormat = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	_texture.Generate(width, height, data);

	stbi_image_free(data);

	return _texture;
}
