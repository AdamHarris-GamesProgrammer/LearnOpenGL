#include "Game.h"

#include <irrKlang.h>

#include "Timer.h"

using namespace irrklang;

glm::vec2 initialBallVel(100.0f, -350.0f);

/* TODO
- Create a Text Class
	- Holds the text to display
	- VAO and VBO
	- Only recreates the VBO when it is needed 
	- Handles color, font, size, transform, and anchor
	- Create an anchoring system where text position is calculated based on where the text origin is
- Make text renderer take in a text object that draws the object 
- Create a button class
- Audio class system?
- Make a input define system for GLFW_KEY_A etc so you can use KEY_A instead
- Texture Atlas to make drawing maps more efficient
- Image UI class
*/

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

bool CheckCollision(GameObject& a, GameObject& b) {
	bool collisionX = a._postion.x + a._size.x >= b._postion.x && b._postion.x + b._size.x >= a._postion.x;
	bool collisionY = a._postion.y + a._size.y >= b._postion.y && b._postion.y + b._size.y >= a._postion.y;

	return collisionX && collisionY;
}

void Game::ActivatePowerup(PowerUp& powerup) {
	switch (powerup._type)
	{
	case SPEED:
		_pBall->_velocity *= 1.2f;
		break;
	case STICKY:
		_pBall->_sticky = true;
		_pPaddle->_color = glm::vec3(1.0f, 0.5f, 1.0f);
		break;
	case PASSTHROUGH:
		_pBall->_passthrough = true;
		_pBall->_color = glm::vec3(1.0f, 0.5f, 0.5f);
		break;
	case PADSIZE_INCREASE:
		_pPaddle->_size.x += 50.0f;
		break;
	case CONFUSE:
		_pPostProcessor->_confuse = true;
		break;
	case CHAOS:
		_pPostProcessor->_chaos = true;
		break;
	}
}

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

ISoundEngine* pSoundEngine = createIrrKlangDevice();

void Game::Init()
{
	pSoundEngine->play2D("Res/Audio/breakout.mp3", true);


	Shader spriteShader = ResourceManager::LoadShader("Res/Shaders/Sprite.vert", "Res/Shaders/Sprite.frag", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
	spriteShader.SetInt("image", 0);
	spriteShader.SetMatrix4("projection", projection);

	_pSpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

	Shader particleShader = ResourceManager::LoadShader("Res/Shaders/particle.vert", "Res/Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Res/Textures/particle.png", true, "particle");
	particleShader.SetInt("sprite", 0);
	particleShader.SetMatrix4("projection", projection);

	_pParticleGenerator = std::make_unique<ParticleGenerator>(particleShader, ResourceManager::GetTexture("particle"), 100);

	ResourceManager::LoadShader("Res/Shaders/Breakout.vert", "Res/Shaders/Breakout.frag", nullptr, "breakout");

	ResourceManager::LoadTexture("Res/Textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("Res/Textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("Res/Textures/block.png", false, "block");
	ResourceManager::LoadTexture("Res/Textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("Res/Textures/paddle.png", true, "paddle");

 	ResourceManager::LoadTexture("Res/Textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("Res/Textures/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("Res/Textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("Res/Textures/powerup_passthrough.png", true, "powerup_passthrough");
	ResourceManager::LoadTexture("Res/Textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("Res/Textures/powerup_sticky.png", true, "powerup_sticky");


	_pCurrentLevel = std::make_unique<GameLevel>();
	_pCurrentLevel->Load("Res/Levels/level1.txt", _width, _height / 2);

	glm::vec2 playerPos = glm::vec2(_width / 2.0f - _playerSize.x / 2.0f, (_height - _playerSize.y) - 20.0f);

	_originalPlayerPos = playerPos;

	_pPaddle = std::make_unique<GameObject>(playerPos, _playerSize, ResourceManager::GetTexture("paddle"));


	float ballRadius = 12.5f;
	glm::vec2 ballPos = playerPos + glm::vec2(_playerSize.x / 2.0f - ballRadius, -ballRadius * 2.0f);

	_originalBallPos = ballPos;

	_pBall = std::make_unique<BallObject>(ballPos, ballRadius, initialBallVel, ResourceManager::GetTexture("face"));

	_pPostProcessor = std::make_unique<PostProcessor>(ResourceManager::GetShader("breakout"), _width, _height);

	_pTextRenderer = std::make_unique<TextRenderer>(_width, _height);
	_pTextRenderer->Load("Res/Fonts/OCRAEXT.TTF", 24);

	livesText = Text("Lives: 3", "generalFont");
	finishText = Text("You Win!", "generalFont");
	menuText = Text("Press Enter to start\nPress W or S to select level", "generalFont");

	finishText.SetPosition(glm::vec2((float)_width / 2, (float)_height / 2));
	finishText.SetScale(2.0f);

	menuText.SetPosition(glm::vec2((float)_width / 2, (_height / 2)));
	menuText.SetAlignment(ALIGN_CENTER);
	menuText.SetScale(2.0f);

	_state = GAME_MENU;
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

	if (_state == GAME_LOSS) {
		if (_keys[GLFW_KEY_ENTER]) {
			_state = GAME_ACTIVE;
			Reset();
		}
	}

	if (_state == GAME_WIN) {
		if (_keys[GLFW_KEY_ENTER]) {
			_state = GAME_MENU;
			Reset();
		}
	}

	if (_state == GAME_MENU) {
		if (_keys[GLFW_KEY_ENTER]) {
			_state = GAME_ACTIVE;
			Reset();
		}
	}
}

void Game::Update(float dt)
{
	if (_state == GAME_ACTIVE) {
		_pBall->Move(dt, _width);

		_pParticleGenerator->Update(dt, *_pBall, 2, glm::vec2(_pBall->_radius / 2.0f));

		UpdatePowerUps(dt);

		CollisionChecks();

		if (_shakeTime > 0.0f) {
			_shakeTime -= dt;
			if (_shakeTime <= 0.0f) _pPostProcessor->_shake = false;
		}
	}
}

void Game::Render()
{
	if (_state == GAME_ACTIVE || _state == GAME_MENU || _state == GAME_WIN || _state == GAME_LOSS) {
		_pPostProcessor->BeginRender();

		_pSpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f), glm::vec2(_width, _height), 0.0f);

		_pCurrentLevel->Draw(*_pSpriteRenderer);

		_pPaddle->Draw(*_pSpriteRenderer);

		_pParticleGenerator->Draw();

		for (PowerUp& p : _powerUps) {
			if (!p._destroyed)
				p.Draw(*_pSpriteRenderer);
		}

		_pBall->Draw(*_pSpriteRenderer);

		_pPostProcessor->EndRender();

		_pPostProcessor->Render(glfwGetTime());

		_pTextRenderer->RenderText(livesText);
	}

	if (_state == GAME_MENU) {
		_pTextRenderer->RenderText(menuText);
	}

	if (_state == GAME_WIN) {
		finishText.SetText("You Won!\nPress Enter to return to menu or ESC to quit");
		_pTextRenderer->RenderText(finishText);
	}

	if (_state == GAME_LOSS) {
		finishText.SetText("You Lost!\nPress Enter to retry or ESC to quit");
		_pTextRenderer->RenderText(finishText);
	}
}

void Game::Reset()
{
	//_state = GAME_LOSS;
	_pBall->Reset(_originalBallPos, initialBallVel);
	_pCurrentLevel->Reset();
	_pPaddle->_postion = _originalPlayerPos;
	_pPaddle->_size = _playerSize;
	_pPostProcessor->ResetState();
	_powerUps.clear();
	_lives = 3;
	livesText.SetText("Lives: 3");
}

void Game::CollisionChecks()
{
	for (GameObject& box : _pCurrentLevel->_bricks) {
		if (!box._destroyed) {
			Collision collision = CheckCollision(*_pBall, box);

			if (std::get<0>(collision)) {
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vec = std::get<2>(collision);

				if (!box._isSolid) {
					box._destroyed = true;
					pSoundEngine->play2D("Res/Audio/bleep.mp3", false);
					SpawnPowerUps(box);

					_pCurrentLevel->_currentActiveBricks--;
					if (_pCurrentLevel->IsCompleted()) _state = GAME_WIN;
				}
				else
				{
					_shakeTime = 0.05f;
					pSoundEngine->play2D("Res/Audio/solid.wav", false);
					_pPostProcessor->_shake = true;
				}


				if (!(_pBall->_passthrough && !box._isSolid)) {
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
	}

	Collision result = CheckCollision(*_pBall, *_pPaddle);
	if (!_pBall->_stuck && std::get<0>(result)) {
		_pBall->_stuck = _pBall->_sticky;

		float centerBoard = _pPaddle->_postion.x + _pPaddle->_size.x / 2.0f;
		float distance = (_pBall->_postion.x + _pBall->_radius) - centerBoard;
		float percentage = distance / (_pPaddle->_size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = _pBall->_velocity;
		_pBall->_velocity.x = initialBallVel.x * percentage * strength;
		_pBall->_velocity.y = -1.0f * abs(_pBall->_velocity.y);
		_pBall->_velocity = glm::normalize(_pBall->_velocity) * glm::length(oldVelocity);
		pSoundEngine->play2D("Res/Audio/bleep.wav", false);
	}

	for (PowerUp& p : _powerUps) {
		if (!p._destroyed) {
			if (p._postion.y >= _height) {
				p._destroyed = true;
			}
			if (CheckCollision(*_pPaddle, p)) {
				ActivatePowerup(p);
				pSoundEngine->play2D("Res/Audio/powerup.wav", false);
				p._destroyed = true;
				p._activated = true;
			}
		}
	}

	if (_pBall->_postion.y >= _height) {
		_lives--;

		std::stringstream ss;
		ss << "Lives: " << _lives;
		livesText.SetText(ss.str());

		if (_lives <= 0) {
			_state = GAME_LOSS;
			//Reset();
		}
		else
		{
			_pPaddle->_postion = _originalPlayerPos;
			_pBall->_postion = _originalBallPos;
			_pBall->_stuck = true;
		}

	}
}

bool ShouldSpawn(unsigned int chance) {
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75)) {
		_powerUps.push_back(PowerUp(SPEED, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block._postion, ResourceManager::GetTexture("powerup_speed")));
	}
	if (ShouldSpawn(75)) {
		_powerUps.push_back(PowerUp(STICKY, glm::vec3(0.5f, 0.5f, 1.0f), 20.0f, block._postion, ResourceManager::GetTexture("powerup_sticky")));
	}
	if (ShouldSpawn(75)) {
		_powerUps.push_back(PowerUp(PASSTHROUGH, glm::vec3(0.5f, 0.5f, 1.0f), 10.0f, block._postion, ResourceManager::GetTexture("powerup_passthrough")));
	}
	if (ShouldSpawn(75)) {
		_powerUps.push_back(PowerUp(PADSIZE_INCREASE, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block._postion, ResourceManager::GetTexture("powerup_increase")));
	}
	if (ShouldSpawn(15)) {
		_powerUps.push_back(PowerUp(CONFUSE, glm::vec3(1.0f, 0.0f, 0.0f), 15.0f, block._postion, ResourceManager::GetTexture("powerup_confuse")));
	}
	if (ShouldSpawn(15)) {
		_powerUps.push_back(PowerUp(CHAOS, glm::vec3(1.0f, 0.0f, 0.0f), 15.0f, block._postion, ResourceManager::GetTexture("powerup_chaos")));
	}
}

bool IsOtherPowerupActive(std::vector<PowerUp>& powerups, PickupType type) {
	for (const PowerUp& p : powerups) {
		if (p._activated && p._type == type) return true;
	}

	return false;
}

void Game::UpdatePowerUps(float dt)
{
	for (PowerUp& p : _powerUps) {
		p._postion += p._velocity * dt;

		if (p._activated)
		{
			p._duration -= dt;

			if (p._duration <= 0.0f) {
				p._activated = false;

				switch (p._type)
				{
				case STICKY:
					if (!IsOtherPowerupActive(_powerUps, STICKY)) {
						_pBall->_sticky = false;
						_pPaddle->_color = glm::vec3(1.0f);
					}
					break;
				case PASSTHROUGH:
					if (!IsOtherPowerupActive(_powerUps, PASSTHROUGH)) {
						_pBall->_passthrough = false;
						_pBall->_color = glm::vec3(1.0f);
					}
					break;
				case CONFUSE:
					if (!IsOtherPowerupActive(_powerUps, CONFUSE)) {
						_pPostProcessor->_confuse = false;
					}
					break;
				case CHAOS:
					if (!IsOtherPowerupActive(_powerUps, CHAOS)) {
						_pPostProcessor->_chaos = false;
					}
					break;
				}
			}
		}
	}

	_powerUps.erase(std::remove_if(_powerUps.begin(), _powerUps.end(), [](const PowerUp& p) {return p._destroyed && !p._activated; }), _powerUps.end());
}
