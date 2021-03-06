#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;

void main() {
	vec4 color = texture(texture1, TexCoords);

	FragColor = color;
}
