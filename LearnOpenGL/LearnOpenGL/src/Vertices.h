#pragma once

float points[] = {
	-0.5f, 0.5f,	1.0f, 0.0f, 0.0f,
	0.5f, 0.5f,		0.0f, 1.0f,	0.0f,
	0.5f, -0.5f,	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,	1.0f, 1.0f, 0.0f
};


//Vertex Positions
//Position, Normal, UV Coordinate
float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,     0.0f, 1.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(4.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float quadVerts[] = {
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
};

float screenQuad[] = {
	-0.3f, 0.5f,	0.0f, 0.0f, //Bottom left
	0.3f, 0.5f,		1.0f, 0.0f, //Bottom Right
	0.3f, 1.0f,		1.0f, 1.0f, //Top Right
	0.3f, 1.0f,		1.0f, 1.0f, //Top Right
	-0.3f, 1.0f,	0.0f, 1.0f, //Top Left
	-0.3f, 0.5f,	0.0f, 0.0f //Bottom Left
};
