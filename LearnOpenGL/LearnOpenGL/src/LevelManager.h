#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <vector>
#include "GameObject.h"
#include "ResourceManager.h"

using json = nlohmann::json;

struct TextureData {
	std::string _name;
	std::string _filepath;
	glm::vec3 _tint;
};

struct TilemapTexture {
	TextureData _data;
	int _tilemapValue;
};

struct Tilemap {
	std::string _filepath;
	std::vector<TilemapTexture> _textures;
	std::vector<std::vector<unsigned int>> _tileValues;
};

struct GameObjectData {
	std::string _uuid;
	std::string _name;
	glm::vec2 _position;
	glm::vec2 _rotation;
	glm::vec2 _scale;
	TextureData _texture;
};

struct SceneData {
	std::string _levelName;
	Tilemap _tilemap;
	std::vector<GameObjectData> _objectData;
};

class LevelManager
{
public:
	static SceneData LoadSceneData(std::string filepath);


private:
	static Tilemap LoadTilemapData(const json& file);
	static TextureData LoadTextureData(const json& file);
	static GameObjectData LoadObjectData(const json& file);
	static bool HasKey(const json& file, std::string key);

};

