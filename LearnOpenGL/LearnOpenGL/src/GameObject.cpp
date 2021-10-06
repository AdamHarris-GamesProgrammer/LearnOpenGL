#include "GameObject.h"

GameObject::GameObject()
	: _postion(0.0f,0.0f), _size(1.0f,1.0f), _velocity(0.0f,0.0), _color(1.0f), _rotation(0.0f), _sprite(), _isSolid(false), _destroyed(false), _ID(UUID::GenerateUUID()) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 _size, Texture2D sprite, glm::vec3 color /*= glm::vec3(1.0)*/, glm::vec2 velocity /*= glm::vec2(0.0f)*/) 
	: _postion(pos), _size(_size), _sprite(sprite), _color(color), _rotation(0.0f), _velocity(velocity), _isSolid(false), _destroyed(false), _ID(UUID::GenerateUUID()) {}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(_sprite, _postion, _size, _rotation, _color);
}
