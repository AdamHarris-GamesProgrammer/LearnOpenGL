#include "Game.h"

glm::vec2 initialBallVel(100.0f, -350.0f);


Direction VectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;

	for (unsigned int i = 0; i < 4; i++) 
	{
		float dot = glm::dot(glm::normalize(target), compass[i]);
		if (dot > max) {
			max = dot;
			bestMatch = i;
		}
	}

	return(Direction)bestMatch;
}

//bool CheckCollision(GameObject& a, GameObject& b) {
//	bool collisionX = a._postion.x + a._size.x >= b._postion.x && b._postion.x + b._size.x >= a._postion.x;
//	bool collisionY = a._postion.y + a._size.y >= b._postion.y && b._postion.y + b._size.y >= a._postion.y;
//
//	return collisionX && collisionY;
//}

Collision CheckCollision(BallObject& a, GameObject& b) {
	glm::vec2 center(a._postion + a._radius);

	glm::vec2 aabb_extents(b._size.x / 2.0f, b._size.y / 2.0f);
	glm::vec2 aabb_center(b._postion.x + aabb_extents.x, b._postion.y + aabb_extents.y);

	glm::vec2 difference = center - aabb_center;



	glm::vec2 clamped = glm::clamp(difference, -aabb_extents, aabb_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	if (glm::length(difference) <= a._radius)
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f));
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

	Shader particleShader = ResourceManager::LoadShader("Res/Shaders/particle.vert", "Res/Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Res/Textures/particle.png", true, "particle");
	particleShader.SetInt("sprite", 0);
	particleShader.SetMatrix4("projection", projection);

	_pParticleGenerator = new ParticleGenerator(particleShader, ResourceManager::GetTexture("particle"), 100);



	ResourceManager::LoadTexture("Res/Textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("Res/Textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("Res/Textures/block.png", false, "block");
	ResourceManager::LoadTexture("Res/Textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("Res/Textures/paddle.png", true, "paddle");
	

	_pCurrentLevel = new GameLevel();
	_pCurrentLevel->Load("Res/Levels/level1.txt", _width, _height / 2);

	glm::vec2 playerPos = glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, (_height - _playerSize.y) - 20.0f);

	_originalPlayerPos = playerPos;

	_pPaddle = new GameObject(playerPos, _playerSize, ResourceManager::GetTexture("paddle"));


	float ballRadius = 12.5f;
	glm::vec2 ballPos = playerPos + glm::vec2(_playerSize.x / 2.0f - ballRadius, -ballRadius * 2.0f);

	_originalBallPos = ballPos;

	_pBall = new BallObject(ballPos, ballRadius, initialBallVel, ResourceManager::GetTexture("face"));

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

	_pParticleGenerator->Update(dt, *_pBall, 2, glm::vec2(_pBall->_radius / 2.0f));


	for (GameObject& box : _pCurrentLevel->_bricks) {
		if (!box._destroyed) {
			Collision collision = CheckCollision(*_pBall, box);

			if (std::get<0>(collision)) {
				if (!box._isSolid) box._destroyed = true;

				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vec = std::get<2>(collision);

				if (dir == LEFT || dir == RIGHT) {
					_pBall->_velocity.x = -_pBall->_velocity.x;

					float penetration = _pBall->_radius - std::abs(diff_vec.x);
					if (dir == LEFT)
						_pBall->_postion.x += penetration;
					else
						_pBall->_postion.x -= penetration;
				}
				else {
					_pBall->_velocity.y = -_pBall->_velocity.y;

					float penetration = _pBall->_radius - std::abs(diff_vec.y);
					if (dir == UP)
						_pBall->_postion.y -= penetration;
					else
						_pBall->_postion.y += penetration;
				}
			}
		}
	}

	Collision result = CheckCollision(*_pBall, *_pPaddle);
	if (!_pBall->_stuck && std::get<0>(result)) {
		float centerBoard = _pPaddle->_postion.x + _pPaddle->_size.x / 2.0f;
		float distance = (_pBall->_postion.x + _pBall->_radius) - centerBoard;
		float percentage = distance / (_pPaddle->_size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = _pBall->_velocity;
		_pBall->_velocity.x = initialBallVel.x * percentage * strength;
		_pBall->_velocity.y = -1.0f * abs(_pBall->_velocity.y);
		_pBall->_velocity = glm::normalize(_pBall->_velocity) * glm::length(oldVelocity);
	}

	if (_pBall->_postion.y >= _height) {
		_pBall->Reset(_originalBallPos, initialBallVel);
		_pCurrentLevel->Reset();
		_pPaddle->_postion = _originalPlayerPos;
	}
}

void Game::Render()
{
	if (_state == GAME_ACTIVE) {
		_pSpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f), glm::vec2(_width, _height), 0.0f);

		_pCurrentLevel->Draw(*_pSpriteRenderer);

		_pPaddle->Draw(*_pSpriteRenderer);

		_pParticleGenerator->Draw();

		_pBall->Draw(*_pSpriteRenderer);
	}
}
