#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_lightPos;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;
out vec3 lightPos;



void main(){
   gl_Position = u_projection * u_view * u_model * vec4(position,1.0);
   fragPos = vec3(u_model * vec4(position, 1.0));
   texCoord = aTexCoord;
   normal = mat3(transpose(inverse(u_model))) * aNormal;
   lightPos = vec3(u_view * vec4(u_lightPos, 1.0));
}