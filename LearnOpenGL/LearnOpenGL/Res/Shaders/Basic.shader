#shader vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

void main(){
   gl_Position = vec4(position,1.0);
   ourColor = aColor;
   texCoord = aTexCoord;
}


#shader fragment

#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

//uniform vec4 u_Color;

uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
}
