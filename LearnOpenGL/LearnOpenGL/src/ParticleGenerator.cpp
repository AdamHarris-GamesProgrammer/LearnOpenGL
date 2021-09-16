#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader _shader, Texture2D _texture, unsigned int _amount)
	: _shader(_shader), _texture(_texture), _amount(_amount)
{
	Init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset /*= glm::vec2(0.0f)*/)
{
	for (unsigned int i = 0; i < newParticles; ++i) {
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(particles[unusedParticle], object, offset);
	}

	for (unsigned int i = 0; i < _amount; ++i) {
		Particle& p = particles[i];
		p.life -= dt;

		if (p.life > 0.0f) {
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5f;
		}
	}
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	_shader.BindShaderProgram();

	for (Particle particle : particles) {
		if (particle.life > 0.0f) {
			_shader.SetFloat2("offset", particle.position);
			_shader.SetFloat4("color", particle.color);
			_texture.Bind();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Init()
{
	// set up mesh and attribute properties
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// create this->amount default particle instances
	for (unsigned int i = 0; i < _amount; ++i)
		particles.push_back(Particle());
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::FirstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < _amount; ++i) {
		if(particles[i].life <= 0.0f) 
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset /*= glm::vec2(0.0f)*/)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.position = object._postion + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object._velocity * 0.1f;
}
