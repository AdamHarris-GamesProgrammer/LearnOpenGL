#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "ResourceManager.h"
#include "Texture2D.h"
#include "Text.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Input.h"
#include "UIObject.h"
#include "Subject.h"
#include "Entity.h"
#include "Event.h"

class Button : public UIObject, public Subject, public Entity
{
public:
	Button();
	~Button();
	Button(glm::vec2 pos, glm::vec2 size = glm::vec2(160.0f, 80.0f));

	Text text;


	void Update();
	void Draw(SpriteRenderer* spriteRend, TextRenderer* textRend);

private:
	Texture2D _texture;
	bool IsPressed();
};

