#shader vertex

#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}


#shader fragment

#version 330 core

uniform vec3 u_lightColor;

out vec4 FragColor;

void main() {
	//FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
	FragColor = vec4(u_lightColor, 1.0);
}