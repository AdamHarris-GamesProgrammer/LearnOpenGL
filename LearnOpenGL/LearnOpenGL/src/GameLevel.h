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

	void LoadTilemap(SceneData data, unsigned int tileSize = 64);
	void LoadTilemap(SceneData data, unsigned int levelWidth, unsigned int levelHeight);

	void Draw(SpriteRenderer& renderer);

	bool IsCompleted();

	void Reset();

	int _currentActiveBricks;

private:
	void Init(unsigned int levelWidth, unsigned int levelHeight);
	void Init(unsigned int tileSize);
	int _amountOfActiveBricks;
	int _tilemapCounter = 0;

	SceneData _data;
	
};

