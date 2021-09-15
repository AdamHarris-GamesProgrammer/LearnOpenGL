#include "BallObject.h"

BallObject::BallObject() : GameObject(), _radius(12.5f), _stuck(true) {}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius  * 2.0f), sprite, glm::vec3(1.0f), velocity), _radius(radius), _stuck(true) {}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth)
{
	if (!_stuck) {
		_postion += _velocity * dt;

		if (_postion.x < 0.0f) {
			_velocity.x = -_velocity.x;
			_postion.x = 0.0f;
		}
		else if (_postion.x + _size.x >= windowWidth) {
			_velocity.x = -_velocity.x;
			_postion.x = windowWidth - _size.x;
		}

		if (_postion.y <= 0.0f) {
			_velocity.y = -_velocity.y;
			_postion.y = 0.0f;
		}
	}

	return _postion;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	_postion = position;
	_velocity = velocity;
	_stuck = true;
}
