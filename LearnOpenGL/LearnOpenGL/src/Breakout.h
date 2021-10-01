#pragma once
#include "Game.h"

#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include "Text.h"
#include "Button.h"

enum Direction {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;


class Breakout : public Game
{

public:
	Breakout(unsigned int width, unsigned int height, GLFWwindow* window);

	void ProcessInput() override;
	void Update() override;
	void RenderGame() override;
	void CollisionChecks() override;
	void OnNotify(const Entity* entity, ObserverEvent event) override;

private:
	std::vector<PowerUp> _powerUps;

	std::unique_ptr<GameObject> _pPaddle = nullptr;

	std::unique_ptr<BallObject> _pBall = nullptr;

	std::unique_ptr<ParticleGenerator> _pParticleGenerator = nullptr;

	std::unique_ptr<PostProcessor> _pPostProcessor = nullptr;

	glm::vec2 _playerSize = glm::vec2(100.0f, 20.0f);
	float _playerVeloicty = 500.0f;
	glm::vec2 initialBallVel = glm::vec2(100.0f, -350.0f);

	unsigned int _lives = 3;
	unsigned int _currentLevelIndex;
	int _amountOfLevels = 4;
	float _durationBetweenSwitchingLevels = 0.1f;
	float _switchTimer = 0.0f;
	float _shakeTime;

	Text livesText;
	Text finishText;
	Text menuText;

	Button* _pPlayButton;
	Button* _pExitButton;

	glm::vec2 _originalPlayerPos;
	glm::vec2 _originalBallPos;

	std::unique_ptr<GameLevel> _pCurrentLevel = nullptr;


	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
	void ActivatePowerup(PowerUp& powerup);
	void Play();
	void Reset();
protected:
	void LoadGameContent() override;

};

