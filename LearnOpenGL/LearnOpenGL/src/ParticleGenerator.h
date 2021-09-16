#pragma once
#include <vector>

#include <gl/glew.h>
#include <glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"


struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader _shader, Texture2D _texture, unsigned int _amount);

	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));

	void Draw();

private:
	std::vector<Particle> particles;
	unsigned int _amount;

	Shader _shader;
	Texture2D _texture;

	unsigned int VAO;

	void Init();

	unsigned int FirstUnusedParticle();
	void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f));

};

