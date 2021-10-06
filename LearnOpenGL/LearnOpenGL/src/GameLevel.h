#pragma once

#include "GameObject.h"
#include "ResourceManager.h"

#include <vector>
#include "LevelManager.h"

class GameLevel
{
public:
	std::vector<GameObject> _bricks;

	std::vector<GameObject> _objects;
	


	GameLevel() {}

	void Load(SceneData& data, unsigned int levelWidth, unsigned int levelHeight);
	void LoadTilemap(SceneData data, unsigned int tileSize = 64);
	void LoadTilemap(SceneData data, unsigned int levelWidth, unsigned int levelHeight);

	void Draw(SpriteRenderer& renderer);

	bool IsCompleted();

	void Reset();

	int _currentActiveBricks;

private:
	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int tileSize);
	int _amountOfActiveBricks;
	
};

