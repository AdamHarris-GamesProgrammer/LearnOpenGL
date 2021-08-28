#shader vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   gl_Position = projection * view * model * vec4(position,1.0);
   texCoord = aTexCoord;
}


#shader fragment

#version 330 core

out vec4 FragColor;

in vec2 texCoord;

//uniform vec4 u_Color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}
