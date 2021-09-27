#include "ResourceManager.h"
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

std::map<std::string, Shader> ResourceManager::_shaders;
std::map<std::string, Texture2D> ResourceManager::_textures;
std::map<std::string, std::map<char, Character>> ResourceManager::_fonts;

unsigned int ResourceManager::_width;
unsigned int ResourceManager::_height;

void ResourceManager::Init(unsigned int width, unsigned int height)
{
	_width = width;
	_height = height;

	Shader spriteShader = ResourceManager::LoadShader("Res/Shaders/Sprite.vert", "Res/Shaders/Sprite.frag", "sprite");
	ResourceManager::LoadFont("Res/Fonts/OCRAEXT.TTF", 24, "generalFont");

	glm::mat4 projection = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
	spriteShader.SetInt("image", 0);
	spriteShader.SetMatrix4("projection", projection);

	Shader particleShader = ResourceManager::LoadShader("Res/Shaders/particle.vert", "Res/Shaders/particle.frag", "particle");
	particleShader.SetInt("sprite", 0);
	particleShader.SetMatrix4("projection", projection);

	Shader textShader = ResourceManager::LoadShader("Res/Shaders/Text.vert", "Res/Shaders/Text.frag", nullptr, "text");
	textShader.SetMatrix4("projection", glm::ortho(0.0f, (float)_width, (float)_height, 0.0f));
	textShader.SetInt("text", 0);
}

Shader ResourceManager::LoadShader(const char* vs, const char* fs, const char* gs, std::string name)
{
	_shaders[name] = LoadShaderFromFile(vs, fs, gs);
	return _shaders[name];
}

Shader ResourceManager::LoadShader(const char* vs, const char* fs, std::string name)
{
	_shaders[name] = LoadShaderFromFile(vs, fs);
	return _shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return _shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
	_textures[name] = LoadTextureFromFile(file);
	return _textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return _textures[name];
}

std::map<char, Character>& ResourceManager::LoadFont(const char* font, int _size, std::string name)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		__debugbreak();
	}

	FT_Face face;
	if (FT_New_Face(ft, font, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		__debugbreak();
	}

	FT_Set_Pixel_Sizes(face, 0, _size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		_fonts[name].insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return _fonts[name];
}

std::map<char, Character>& ResourceManager::GetFont(std::string name)
{
	return _fonts[name];
}

void ResourceManager::Clear()
{
	for (auto iter : _shaders)
		glDeleteProgram(iter.second.GetID());
	for (auto iter : _textures)
		glDeleteTextures(1, &iter.second._ID);
	_fonts.clear();
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

Texture2D ResourceManager::LoadTextureFromFile(const char* file)
{
	Texture2D _texture;

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (nrChannels == 4) {
		_texture._imageFormat = GL_RGBA;
		_texture._internalFormat = GL_RGBA;
	}

	_texture.Generate(width, height, data);

	stbi_image_free(data);

	return _texture;
}
