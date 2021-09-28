#include "Button.h"

Button::Button()
{
	_position = glm::vec2(0.0f);
	_size = glm::vec2(160.0f, 80.0f);
	text = Text("Button");
	_texture = ResourceManager::GetTexture("block");
	text.Finalize();
	text._pParent = this;
	UIObject(_position);
}

Button::Button(glm::vec2 pos, glm::vec2 size /*= glm::vec2(160.0f, 80.0f)*/)
{
	_position = pos;
	_size = size;
	text = Text("Button");
	_texture = ResourceManager::GetTexture("block");
	text._pParent = this;
	UIObject(_position);
}

Button::Button(glm::vec2 pos, glm::vec2 size /*= glm::vec2(160.0f, 80.0f)*/)
{

}

bool Button::IsPressed()
{
	float left = _position.x;
	float right = _position.x + _size.x;
	float top = _position.y;
	float bottom = _position.y + _size.y;

	glm::vec2 cursorPosition = Input::GetMousePosition();

	if (cursorPosition.x >= left && cursorPosition.x <= right && cursorPosition.y >= top && cursorPosition.y <= bottom) {
		return true;
	}

	return false;
}

void Button::Update()
{
	if (Input::IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (IsPressed()) {
			//TODO: Loop through listeners
		}
	}
}

void Button::Draw(SpriteRenderer* spriteRend, TextRenderer* textRend)
{
	PropagatePosition();
	spriteRend->DrawSprite(_texture, _screenPosition, _size);
	textRend->RenderText(text);
}

Button::~Button()
{

}
