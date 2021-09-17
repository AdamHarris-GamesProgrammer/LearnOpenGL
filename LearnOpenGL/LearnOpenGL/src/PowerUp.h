#pragma once
#include "GameObject.h"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject {
public:
	std::string _type;
	float _duration;
	bool _activated;

	PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY), _type(type), _duration(duration), _activated(false) {

	}

};