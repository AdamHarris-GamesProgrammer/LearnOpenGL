#pragma once
#include "GameObject.h"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

enum PickupType {
	SPEED,
	STICKY,
	PASSTHROUGH,
	PADSIZE_INCREASE,
	CONFUSE,
	CHAOS
};


class PowerUp : public GameObject {
public:
	PickupType _type;
	float _duration;
	bool _activated;

	PowerUp(PickupType type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY), _type(type), _duration(duration), _activated(false) {

	}

};