#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct DirectionalLight : public Light{
	glm::vec3 direction;

	DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) {
		direction = dir;
		ambient = amb;
		diffuse = diff;
		specular = spec;
	}
};

struct PointLight : public Light {
	glm::vec3 position;

	float linear;
	float constant;
	float quadratic;

	PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float lin, float cons, float quad) {
		position = pos;
		ambient = amb;
		diffuse = diff;
		specular = spec;
		linear = lin;
		constant = cons;
		quadratic = quad;
	}
};