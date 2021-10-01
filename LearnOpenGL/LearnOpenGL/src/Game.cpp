#include "Game.h"


/* TODO
- Audio class system?
- Texture Atlas to make drawing maps more efficient
- Image UI class
- Parenting of object
- Buttons need things like on hover and on click tints and textures
- Entity component system
- Level saving loader system
	- Objects need a UUID
- IMGUI implementation
- Divide the screen into 3 sections; UI, scene and game views
- Make Game class a base class
- Expand texture loading system to take in a structure that contains the path to the id value, texture, and color
- Implement features for align in top and bottom
*/



Game::Game(unsigned int width, unsigned int height, GLFWwindow* window)
	: _width(width), _height(height), _state(GAME_ACTIVE), _pWindow(window)
{
	ResourceManager::Init(_width, _height);
	Init();
	LoadGameContent();
}

Game::~Game()
{
}

void Game::Init()
{
	UUID::Init();

	Input::SetCursorVisibility(CursorState::CURSOR_NORMAL);
	_pSpriteRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));
	_pTextRenderer = std::make_unique<TextRenderer>();
	pSoundEngine = createIrrKlangDevice();
}

void Game::LoadGameContent()
{

}

void Game::ProcessInput()
{

}

void Game::Update()
{

}

void Game::RenderGame()
{

}

void Game::RenderScene() {

}

void Game::RenderSceneUI() {

}

void Game::CollisionChecks()
{

}

void Game::OnNotify(const Entity& entity, ObserverEvent event)
{

}

void Game::OnNotify(ObserverEvent event)
{

}

void Game::OnNotify(const Entity* entity, ObserverEvent event)
{

}