#include "Button.h"

Button::Button()
{
	__debugbreak();

	//position = glm::vec2(0.0f);
	//size = glm::vec2(160.0f, 80.0f);
	//text = Text("Button", "generalFont");
	//texture = ResourceManager::GetTexture("block");
	//text.Finalize();
}

Button::Button(glm::vec2 pos, glm::vec2 size /*= glm::vec2(160.0f, 80.0f)*/)
{
	_position = pos;
	_size = size;
	text = Text("Button", "generalFont");
	_texture = ResourceManager::GetTexture("block");
}

bool Button::IsPressed(glm::vec2 cursorPosition)
{
	float left = _position.x;
	float right = _position.x + _size.x;
	float top = _position.y;
	float bottom = _position.y + _size.y;

	if (cursorPosition.x >= left && cursorPosition.x <= right && cursorPosition.y >= top && cursorPosition.y <= bottom) {
		std::cout << "Button Pressed" << std::endl;
		return true;
	}

	return false;
}

void Button::Draw(SpriteRenderer* spriteRend, TextRenderer* textRend)
{
	spriteRend->DrawSprite(_texture, _position, _size);
	textRend->RenderText(text);
}

Button::~Button()
{

}
