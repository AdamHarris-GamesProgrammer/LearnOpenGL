#pragma once

#include <GL/glew.h>
#include <glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	glm::vec2 _postion, _size, _velocity;
	glm::vec3 _color;

	float _rotation;
	bool _isSolid;
	bool _destroyed;

	Texture2D _sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0), glm::vec2 velocity = glm::vec2(0.0f));

	virtual void Draw(SpriteRenderer& renderer);

};

