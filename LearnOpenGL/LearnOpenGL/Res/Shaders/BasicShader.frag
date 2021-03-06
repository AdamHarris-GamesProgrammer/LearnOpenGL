#version 330 core
#include "LightUtilities.glsl" //! #include "LightUtilities.glsl"

#define NR_POINT_LIGHTS 4

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

uniform vec3 u_viewPos;
uniform Material u_material;

uniform DirLight u_dirLight;
uniform PointLight u_pointLight[NR_POINT_LIGHTS];

out vec4 FragColor;

void main() {
	u_material;
	u_pointLight[0];
	u_pointLight[1];
	u_pointLight[2];
	u_pointLight[3];

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(u_viewPos - fragPos);

	vec3 result = CalcDirLight(u_dirLight, norm, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(u_pointLight[i], norm, fragPos, viewDir);
	}

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
