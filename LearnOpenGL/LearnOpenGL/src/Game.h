#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <tuple>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"

#include "BallObject.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;



class Game
{
public:
	GameState _state;
	bool _keys[1024];
	unsigned int _width, _height;

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

private:
	SpriteRenderer* _pSpriteRenderer = nullptr;
	GameLevel* _pCurrentLevel = nullptr;

	GameObject* _pPaddle = nullptr;

	BallObject* _pBall = nullptr;

	glm::vec2 _playerSize = glm::vec2(100.0f, 20.0f);
	float _playerVeloicty = 500.0f;

	glm::vec2 _originalPlayerPos;
	glm::vec2 _originalBallPos;
};

