#include "LevelManager.h"

SceneData LevelManager::LoadSceneData(std::string filepath)
{
	SceneData scene;

	std::ifstream inFile(filepath);

	//Creates the json reader
	json jsonFile;

	//Reads the data from the file into the json object
	inFile >> jsonFile;

	if (HasKey(jsonFile, "scenename")) {
		scene._levelName = jsonFile["scenename"];
	}

	if (HasKey(jsonFile, "tilemap")) {
		json tilemap = jsonFile["tilemap"];
		scene._tilemap = LoadTilemapData(tilemap);
	}

	if (HasKey(jsonFile, "gameobjects")) {
		json objects = jsonFile["gameobjects"];

		int size = objects.size();
		for (int i = 0; i < size; i++) {
			json currentObject = objects.at(i);
			scene._objectData.push_back(LoadObjectData(currentObject));
		}
	}

	return scene;
}

Tilemap LevelManager::LoadTilemapData(const json& file)
{
	Tilemap map;

	if (HasKey(file, "filepath")) {
		map._filepath = file["filepath"];

		unsigned int tileCode;
		std::string line;
		std::ifstream fstream(map._filepath);

		if (fstream) {
			while (std::getline(fstream, line)) {
				std::istringstream sstream(line);
				std::vector<unsigned int> row;
				while (sstream >> tileCode) {
					row.push_back(tileCode);
				}
				map._tileValues.push_back(row);
			}
		}

		if (HasKey(file, "textures")) {
			json textures = file["textures"];

			int size = textures.size();
			map._textures.resize(size);
			for (int i = 0; i < size; i++) {
				json currentTextures = textures.at(i);
				map._textures.at(i)._data = LoadTextureData(currentTextures);
				map._textures.at(i)._tilemapValue = currentTextures["tilemapValue"];

				ResourceManager::LoadTexture(map._textures.at(i)._data._filepath.c_str(), map._textures.at(i)._data._name);
			}
		}
	}
	else {
		std::cout << "[ERROR: LevelManager]: No filepath found for passed in tilemap" << std::endl;
	}

	return map;
}

glm::vec3 LoadVec3(const std::vector<float>& vec) {
	return glm::vec3(vec[0], vec[1], vec[2]);
}

glm::vec2 LoadVec2(const std::vector<float>& vec) {
	return glm::vec2(vec[0], vec[1]);
}

TextureData LevelManager::LoadTextureData(const json& file)
{
	TextureData texture;

	if (HasKey(file, "name")) {
		texture._name = file["name"];
	}

	if (HasKey(file, "filepath")) {
		texture._filepath = file["filepath"];
	}

	if (HasKey(file, "tint")) {
		texture._tint = LoadVec3(file["tint"]);
	}

	return texture;
}

GameObjectData LevelManager::LoadObjectData(const json& file)
{
	GameObjectData object;

	if (HasKey(file, "uuid")) {
		object._uuid = file["uuid"];
	}
	else { 
		object._uuid = UUID::GenerateUUID();
	}

	if (HasKey(file, "name")) {
		object._name = file["name"];
	}

	if (HasKey(file, "position")) {
		object._position = LoadVec2(file["position"]);
	}

	if (HasKey(file, "rotation")) {
		object._rotation = LoadVec2(file["rotation"]);
	}

	if (HasKey(file, "scale")) {
		object._scale = LoadVec2(file["scale"]);
	}

	if (HasKey(file, "texture")) {
		json texture = file["texture"];
		object._texture = LoadTextureData(texture);
	}

	return object;
}

bool LevelManager::HasKey(const json& file, std::string key)
{
	if (file.contains(key)) return true;
	return false;
}
