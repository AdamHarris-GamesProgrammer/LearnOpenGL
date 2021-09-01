#pragma once
#include "glm.hpp"
#include <string>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoord;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh
{
};

