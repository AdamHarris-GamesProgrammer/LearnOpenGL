#include "Game.h"

bool CheckCollision(GameObject& a, GameObject& b) {
	bool collisionX = a._postion.x + a._size.x >= b._postion.x && b._postion.x + b._size.x >= a._postion.x;
	bool collisionY = a._postion.y + a._size.y >= b._postion.y && b._postion.y + b._size.y >= a._postion.y;

	return collisionX && collisionY;
}

float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

bool CheckCollision(BallObject& a, GameObject& b) {
	glm::vec2 center(a._postion + a._radius);

	glm::vec2 aabb_extents(b._size.x / 2.0f, b._size.y / 2.0f);
	glm::vec2 aabb_center(b._postion.x + aabb_extents.x, b._postion.y + aabb_extents.y);

	glm::vec2 difference = center - aabb_center;

	glm::vec2 clamped = glm::clamp(difference, -aabb_extents, aabb_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	return glm::length(difference) < a._radius;
}

Game::Game(unsigned int width, unsigned int height)
	: _width(width), _height(height), _state(GAME_ACTIVE)
{
	_width = width;
	_height = height;
}

Game::~Game()
{

}

void Game::Init()
{
	Shader spriteShader = ResourceManager::LoadShader("Res/Shaders/Sprite.vert", "Res/Shaders/Sprite.frag", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
	spriteShader.SetInt("image", 0);
	spriteShader.SetMatrix4("projection", projection);

	_pSpriteRenderer = new SpriteRenderer(spriteShader);

	ResourceManager::LoadTexture("Res/Textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("Res/Textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("Res/Textures/block.png", false, "block");
	ResourceManager::LoadTexture("Res/Textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("Res/Textures/paddle.png", true, "paddle");

	_pCurrentLevel = new GameLevel();
	_pCurrentLevel->Load("Res/Levels/level1.txt", _width, _height / 2);

	glm::vec2 playerPos = glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, (_height - _playerSize.y) - 20.0f);

	_pPaddle = new GameObject(playerPos, _playerSize, ResourceManager::GetTexture("paddle"));


	float ballRadius = 12.5f;
	glm::vec2 ballPos = playerPos + glm::vec2(_playerSize.x / 2.0f - ballRadius, -ballRadius * 2.0f);

	_pBall = new BallObject(ballPos, ballRadius, glm::vec2(100.0f, -350.0f), ResourceManager::GetTexture("face"));

}

void Game::ProcessInput(float dt)
{
	if (_state == GAME_ACTIVE) {
		float velocity = _playerVeloicty * dt;

		if (_keys[GLFW_KEY_A]) {
			if (_pPaddle->_postion.x >= 0.0f) {
				_pPaddle->_postion.x -= velocity;
				if (_pBall->_stuck) _pBall->_postion.x -= velocity;
			}
		}
		if (_keys[GLFW_KEY_D]) {
			if (_pPaddle->_postion.x <= _width - _pPaddle->_size.x) {
				_pPaddle->_postion.x += velocity;
				if (_pBall->_stuck) _pBall->_postion.x += velocity;
			}
		}

		if (_keys[GLFW_KEY_SPACE])
			_pBall->_stuck = false;
	}
}

void Game::Update(float dt)
{
	_pBall->Move(dt, _width);


	for (GameObject& box : _pCurrentLevel->_bricks) {
		if (!box._destroyed) {
			if (CheckCollision(*_pBall, box)) {
				if (!box._isSolid) {
					box._destroyed = true;
				}
			}
		}
	}
}

void Game::Render()
{
	if (_state == GAME_ACTIVE) {
		_pSpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f), glm::vec2(_width, _height), 0.0f);

		_pCurrentLevel->Draw(*_pSpriteRenderer);

		_pPaddle->Draw(*_pSpriteRenderer);

		_pBall->Draw(*_pSpriteRenderer);
	}
}
