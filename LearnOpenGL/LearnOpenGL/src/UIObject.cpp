#include "UIObject.h"

UIObject::UIObject()
	: _position(glm::vec2(0.0f)), _scale(1.0f), _alignment(ALIGN_LEFT)
{

}

UIObject::UIObject(glm::vec2 position, float scale /*= 1.0f*/, Alignment alignment /*= ALIGN_CENTER*/)
	: _position(position), _scale(scale), _alignment(alignment) {}

void UIObject::SetPosition(glm::vec2 pos)
{
	_position = pos;
}

void UIObject::SetPosition(float x, float y)
{
	_position = glm::vec2(x, y);
}

void UIObject::SetScale(float s)
{
	_scale = s;
}

void UIObject::SetAlignment(Alignment align)
{
	_alignment = align;
}

void UIObject::PropagatePosition()
{
	UIObject* parent = _pParent;
	_screenPosition = _position;

	while (parent != nullptr) {
		_screenPosition += parent->_position;

		if(_alignment == ALIGN_CENTER)
			_screenPosition += parent->_size / 2.0f;
		parent = parent->_pParent;
	}
}
