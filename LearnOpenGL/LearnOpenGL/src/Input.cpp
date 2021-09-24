#include "Input.h"

bool Input::_mouseButtons[16];
bool Input::_keys[1024];
glm::vec2 Input::_mousePosition;
GLFWwindow* Input::_pWindow;

bool Input::IsKeyDown(KeyCode code)
{
	return (_keys[code] == InputState::BUTTON_DOWN);
}

bool Input::IsKeyUp(KeyCode code)
{
	return (_keys[code] == InputState::BUTTON_UP);
}

void Input::SetCursorVisibility(CursorState state)
{
	glfwSetInputMode(_pWindow, GLFW_CURSOR, state);
}

bool Input::IsMouseButtonDown(MouseButton button)
{
	return (_mouseButtons[button] == InputState::BUTTON_DOWN);
}

bool Input::IsMouseButtonUp(MouseButton button)
{
	return (_mouseButtons[button] == InputState::BUTTON_UP);
}

glm::vec2 Input::GetMousePosition()
{
	return _mousePosition;
}

void Input::SetMousePosition(glm::vec2 pos)
{
	_mousePosition = pos;
}

void Input::SetMouseButton(MouseButton button, InputState state)
{
	_mouseButtons[button] = state;
}

void Input::SetKey(KeyCode code, InputState state)
{
	_keys[code] = state;
}