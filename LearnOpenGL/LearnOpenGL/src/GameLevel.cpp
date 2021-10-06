#include "GameLevel.h"

#include <fstream>
#include <string>

void GameLevel::LoadTilemap(SceneData data, unsigned int tileSize)
{
	_tilemapCounter = 0;

	_bricks.clear();
	_data = data;
	if (data._tilemap._tileValues.size() > 0) Init(tileSize);

	_amountOfActiveBricks = _tilemapCounter;
	//_amountOfActiveBricks = 1;
	_currentActiveBricks = _amountOfActiveBricks;
}

void GameLevel::LoadTilemap(SceneData data, unsigned int levelWidth, unsigned int levelHeight)
{
	_tilemapCounter = 0;

	_bricks.clear();
	_data = data;
	if (data._tilemap._tileValues.size() > 0) Init(levelWidth, levelHeight);

	_amountOfActiveBricks = _tilemapCounter;
	//_amountOfActiveBricks = 1;
	_currentActiveBricks = _amountOfActiveBricks;
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
	glm::vec2 _size(unit_width, unit_height);


	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			unsigned int tileValue = _data._tilemap._tileValues[y][x];
			if (tileValue == 0) continue;

			glm::vec2 pos(unit_width * x, unit_height * y);
			GameObject obj(pos, _size, ResourceManager::GetTexture(_data._tilemap._textures[tileValue]._data._name),
				_data._tilemap._textures[tileValue]._data._tint);

			if (tileValue == 1) {
				obj.SetSolid(true);
			}

			_tilemapCounter++;
			_bricks.push_back(obj);
		}
	}

}

void GameLevel::Init(unsigned int tileSize)
{
	glm::vec2 _size = glm::vec2(tileSize);

	for (unsigned int y = 0; y < _data._tilemap._tileValues.size(); y++) {
		for (unsigned int x = 0; x < _data._tilemap._tileValues[0].size(); x++) {
			unsigned int tileValue = _data._tilemap._tileValues[y][x];
			if(tileValue == 0) continue;


			glm::vec2 pos(tileSize * x, tileSize * y);
			GameObject obj(pos, _size, ResourceManager::GetTexture(_data._tilemap._textures[tileValue]._data._name),
				_data._tilemap._textures[tileValue]._data._tint);

			if (tileValue == 1) {
				obj.SetSolid(true);
			}

			_tilemapCounter++;
			_bricks.push_back(obj);
		}
	}
}
