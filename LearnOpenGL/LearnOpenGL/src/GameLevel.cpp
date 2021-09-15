#include "GameLevel.h"

#include <fstream>
#include <string>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	_bricks.clear();

	unsigned int tileCode;

	GameLevel level;

	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode) {
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
	}
	if (tileData.size() > 0) Init(tileData, levelWidth, levelHeight);

}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (auto& go : _bricks) {
		if (go._destroyed) continue;

		go.Draw(renderer);
	}
}

bool GameLevel::IsCompleted()
{
	return false;
}

void GameLevel::Reset()
{
	for (auto& go : _bricks) {
		go._destroyed = false;
	}
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();

	float unit_width = levelWidth / (float)width;
	float unit_height = levelHeight / (float)height;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);

			unsigned int tileValue = tileData[y][x];

			if (tileValue == 1) {
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj._isSolid = true;
				_bricks.push_back(obj);
			}
			else if (tileValue > 1) {
				glm::vec3 color = glm::vec3(1.0f);

				if (tileValue == 2) color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileValue == 3) color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileValue == 4) color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileValue == 5) color = glm::vec3(1.0f, 0.5f, 0.0f);

				_bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}


		}
	}
}
