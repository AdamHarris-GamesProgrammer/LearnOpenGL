#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <irrKlang.h>

#include <tuple>
#include <memory>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include "TextRenderer.h"
#include "Text.h"
#include "Button.h"
#include "Input.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSS
};

enum Direction {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

using namespace irrklang;

class Game
{
public:
	GLFWwindow* _pWindow;

	GameState _state;
	bool _keys[1024];
	unsigned int _width, _height;

	void ActivatePowerup(PowerUp& powerup);
	Game(unsigned int width, unsigned int height, GLFWwindow* window);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Reset();
	void CollisionChecks();


	std::vector<PowerUp> _powerUps;

	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);

	void SetMousePos(glm::vec2 pos);
	void SetMousePressed(bool pressed);
private:
	std::unique_ptr<SpriteRenderer> _pSpriteRenderer = nullptr;
	std::unique_ptr<GameLevel> _pCurrentLevel = nullptr;

	std::unique_ptr<GameObject> _pPaddle = nullptr;

	std::unique_ptr<BallObject> _pBall = nullptr;

	std::unique_ptr<TextRenderer> _pTextRenderer = nullptr;

	std::unique_ptr<ParticleGenerator> _pParticleGenerator = nullptr;

	std::unique_ptr<PostProcessor> _pPostProcessor = nullptr;

	glm::vec2 _playerSize = glm::vec2(100.0f, 20.0f);
	float _playerVeloicty = 500.0f;

	glm::vec2 _originalPlayerPos;
	glm::vec2 _originalBallPos;

	unsigned int _lives = 3;
	unsigned int _currentLevelIndex;
	int _amountOfLevels = 4;
	float _durationBetweenSwitchingLevels = 0.1f;
	float _switchTimer = 0.0f;

	float _shakeTime;

	glm::vec2 _mousePos;
	bool _mousePressed;

	Text livesText;
	Text finishText;
	Text menuText;

	Button* _pPlayButton;
	Button* _pExitButton;

	ISoundEngine* pSoundEngine;
	
};

