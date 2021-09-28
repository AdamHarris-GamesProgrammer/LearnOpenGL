#pragma once

#include <glm.hpp>


enum Alignment {
	ALIGN_TOP_LEFT,
	ALIGN_TOP_CENTER,
	ALIGN_TOP_RIGHT,
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
	ALIGN_BOTTOM_LEFT,
	ALIGN_BOTTOM_CENTER,
	ALIGN_BOTTOM_RIGHT
};

class UIObject
{
public:
	UIObject();
	UIObject(glm::vec2 position, float scale = 1.0f, Alignment alignment = ALIGN_CENTER);


	void SetPosition(glm::vec2 pos);
	void SetPosition(float x, float y);
	void SetScale(float s);
	void SetAlignment(Alignment align);

	UIObject* _pParent = nullptr;
private:


protected:
	void PropagatePosition();

	Alignment _alignment;
	glm::vec2 _size;

	glm::vec2 _position;
	glm::vec2 _screenPosition;
	float _scale;

};

