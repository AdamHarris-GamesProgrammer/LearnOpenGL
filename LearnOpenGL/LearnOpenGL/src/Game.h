#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

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
};

