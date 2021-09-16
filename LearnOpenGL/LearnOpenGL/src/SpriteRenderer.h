#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture2D.h"

#include "ErrorHandling.h"

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& _shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& _texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader _shader;
	unsigned int quadVAO;

	void InitRenderData();
};

