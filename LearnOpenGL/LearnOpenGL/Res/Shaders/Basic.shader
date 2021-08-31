#shader vertex

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


#shader fragment

#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Light u_light;

out vec4 FragColor;
//uniform vec4 u_Color;

//uniform sampler2D texture1;
//uniform sampler2D texture2;



void main() {
	//FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);

	float ambientStrength = 0.1;
	float specularStrength = 0.5;


	vec3 ambient = u_light.ambient * u_material.ambient;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);



	vec3 viewDir = normalize(u_viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * (spec * u_material.specular);

	vec3 result = (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0);
}
