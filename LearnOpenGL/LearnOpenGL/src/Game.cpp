#include "Game.h"

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

	ResourceManager::LoadTexture("Res/Textures/container2.png", true, "default");
}

void Game::ProcessInput(float dt)
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{
	Texture2D texture = ResourceManager::GetTexture("default");
	_pSpriteRenderer->DrawSprite(texture, glm::vec2(500.0f, 200.f), glm::vec2(32.0f, 32.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}
