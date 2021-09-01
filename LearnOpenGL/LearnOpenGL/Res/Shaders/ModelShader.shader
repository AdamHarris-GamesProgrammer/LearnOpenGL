#shader vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 texCoord;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    texCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material u_material;

void main()
{
    FragColor = texture(u_material.texture_diffuse1, TexCoords);
}