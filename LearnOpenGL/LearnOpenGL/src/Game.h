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
#include "TextRenderer.h"
#include "Input.h"
#include "Observer.h"
#include "UUID.h"
#include "Timer.h"
#include "Time.h"


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSS
};

using namespace irrklang;

class Game : public Observer
{
public:
	GameState _state;
	
	Game(unsigned int width, unsigned int height, GLFWwindow* window);
	~Game();
	
	virtual void ProcessInput();
	virtual void Update();
	virtual void RenderGame();
	virtual void RenderScene();
	virtual void RenderSceneUI();

	virtual void CollisionChecks();

	void OnNotify(const Entity& entity, ObserverEvent event) override;
	void OnNotify(ObserverEvent event) override;
	void OnNotify(const Entity* entity, ObserverEvent event) override;

private:
	void Init();

protected:
	unsigned int _width, _height;

	virtual void LoadGameContent();

	ISoundEngine* pSoundEngine;

	GLFWwindow* _pWindow;

	std::unique_ptr<SpriteRenderer> _pSpriteRenderer = nullptr;
	std::unique_ptr<TextRenderer> _pTextRenderer = nullptr;
};

