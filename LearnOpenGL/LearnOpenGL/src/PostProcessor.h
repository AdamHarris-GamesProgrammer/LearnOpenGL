#pragma once

#include <gl/glew.h>
#include <glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class PostProcessor
{
public:
	Shader _postProcessingShader;
	Texture2D _texture;
	unsigned int _width, _height;

	bool _confuse, _chaos, _shake;

	PostProcessor(Shader shader, unsigned int width, unsigned int height);

	void BeginRender();
	void EndRender();
	void Render(float time);
private:
	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;

	void InitRenderData();

};

