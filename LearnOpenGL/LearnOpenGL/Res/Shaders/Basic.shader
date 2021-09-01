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

#define NR_POINT_LIGHTS 4

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

uniform vec3 u_viewPos;
uniform Material u_material;

uniform DirLight u_dirLight;
uniform PointLight u_pointLight[NR_POINT_LIGHTS];
//uniform PointLight u_pointLight;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	u_material;
	u_pointLight[0];
	u_pointLight[1];
	u_pointLight[2];
	u_pointLight[3];

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(u_viewPos - fragPos);

	vec3 result = CalcDirLight(u_dirLight, norm, viewDir);

	//for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		//result += CalcPointLight(u_pointLight[i], norm, fragPos, viewDir);
	//}

	result += CalcPointLight(u_pointLight[0], norm, fragPos, viewDir);
	result += CalcPointLight(u_pointLight[1], norm, fragPos, viewDir);
	result += CalcPointLight(u_pointLight[2], norm, fragPos, viewDir);
	result += CalcPointLight(u_pointLight[3], norm, fragPos, viewDir);


	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(u_material.specular, texCoord));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(u_material.specular, texCoord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
