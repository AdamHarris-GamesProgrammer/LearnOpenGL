#pragma once

#include <GL/glew.h>
#include <glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "UUID.h"
#include <string>

class GameObject
{
public:
	glm::vec2 _postion, _size, _velocity;
	glm::vec3 _color;

	float _rotation;


	Texture2D _sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 _size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0), glm::vec2 velocity = glm::vec2(0.0f));

	virtual void Draw(SpriteRenderer& renderer);

	inline std::string ID() const {
		return _ID;
	}

	inline bool IsSolid() const {
		return _isSolid;
	}

	inline bool IsDestroyed() const {
		return _destroyed;
	}

	inline void SetDestroyed(const bool val) {
		_destroyed = val;
	}

	inline void SetSolid(const bool val) {
		_isSolid = val;
	}

private:
	std::string _ID;
	bool _isSolid;
	bool _destroyed;


};

