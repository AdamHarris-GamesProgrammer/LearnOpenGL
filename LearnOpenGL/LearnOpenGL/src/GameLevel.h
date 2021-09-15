#pragma once

#include "GameObject.h"
#include "ResourceManager.h"

#include <vector>

class GameLevel
{
public:
	std::vector<GameObject> _bricks;

	GameLevel() {}

	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

	void Draw(SpriteRenderer& renderer);

	bool IsCompleted();

	void Reset();

private:
	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

