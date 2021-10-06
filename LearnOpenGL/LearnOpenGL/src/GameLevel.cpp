#include "GameLevel.h"

#include <fstream>
#include <string>

void GameLevel::LoadTilemap(SceneData data, unsigned int tileSize)
{
	_bricks.clear();
	_data = data;
	if (data._tilemap._tileValues.size() > 0) Init(tileSize);
}

void GameLevel::LoadTilemap(SceneData data, unsigned int levelWidth, unsigned int levelHeight)
{
	_bricks.clear();
	_data = data;
	if (data._tilemap._tileValues.size() > 0) Init(levelWidth, levelHeight);
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (auto& go : _bricks) {
		if (go.IsDestroyed()) continue;

		go.Draw(renderer);
	}
}

bool GameLevel::IsCompleted()
{
	return _currentActiveBricks <= 0;
}

void GameLevel::Reset()
{
	for (auto& go : _bricks) {
		go.SetDestroyed(false);
	}
	_currentActiveBricks = _amountOfActiveBricks;
}

void GameLevel::Init(unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int height = _data._tilemap._tileValues.size();
	unsigned int width = _data._tilemap._tileValues[0].size();

	float unit_width = levelWidth / (float)width;
	float unit_height = levelHeight / (float)height;

	int counter = 0;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 _size(unit_width, unit_height);

			unsigned int tileValue = _data._tilemap._tileValues[y][x];

			if (tileValue == 1) {
				GameObject obj(pos, _size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.SetSolid(true);
				_bricks.push_back(obj);
			}
			else if (tileValue > 1) {
				glm::vec3 color = glm::vec3(1.0f);

				if (tileValue == 2) color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileValue == 3) color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileValue == 4) color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileValue == 5) color = glm::vec3(1.0f, 0.5f, 0.0f);

				_bricks.push_back(GameObject(pos, _size, ResourceManager::GetTexture("block"), color));

				counter++;
			}
		}
	}
	_amountOfActiveBricks = counter;
	//_amountOfActiveBricks = 1;
	_currentActiveBricks = _amountOfActiveBricks;
}

void GameLevel::Init(unsigned int tileSize)
{
	glm::vec2 _size = glm::vec2(tileSize);

	int counter = 0;
	for (unsigned int y = 0; y < _data._tilemap._tileValues.size(); y++) {
		for (unsigned int x = 0; x < _data._tilemap._tileValues[0].size(); x++) {
			glm::vec2 pos(tileSize * x, tileSize * y);
			

			unsigned int tileValue = _data._tilemap._tileValues[y][x];

			if (tileValue == 1) {
				GameObject obj(pos, _size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.SetSolid(true);
				_bricks.push_back(obj);
			}
			else if (tileValue > 1) {
				glm::vec3 color = glm::vec3(1.0f);

				if (tileValue == 2) color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileValue == 3) color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileValue == 4) color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileValue == 5) color = glm::vec3(1.0f, 0.5f, 0.0f);

				_bricks.push_back(GameObject(pos, _size, ResourceManager::GetTexture("block"), color));

				counter++;
			}
		}
	}
	_amountOfActiveBricks = counter;
	_currentActiveBricks = _amountOfActiveBricks;
}
