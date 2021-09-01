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
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

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
uniform Light u_light;

out vec4 FragColor;

//uniform sampler2D texture1;
//uniform sampler2D texture2;



void main() {
	vec3 result;

	//FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
	vec3 lightDir = normalize(u_light.position - fragPos);

	float theta = dot(lightDir, normalize(-u_light.direction));
	float epsilon = u_light.cutoff - u_light.outerCutoff;
	//float intensity = clamp((theta - u_light.outerCutoff) / epsilon, 0.0, 1.0);
	float intensity = smoothstep(0.0, 1.0, (theta - u_light.outerCutoff) / epsilon);


	if (theta > u_light.cutoff) {
		float ambientStrength = 0.1;
		float specularStrength = 0.5;


		vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, texCoord));

		vec3 norm = normalize(normal);


		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, texCoord));

		vec3 viewDir = normalize(u_viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
		vec3 specularMap = vec3(texture(u_material.specular, texCoord));
		vec3 specular = u_light.specular * spec * specularMap;

		//EMISSION MAP
		//vec2 moddedTexCoords = texCoord;
		//moddedTexCoords.x = moddedTexCoords.x + 0.045f;
		//vec3 emissionMap = vec3(texture(u_material.emission, moddedTexCoords));
		//vec3 emission = emissionMap;
		//vec3 emissionMask = step(vec3(1.0f), vec3(1.0f) - specularMap);
		//emission = emission * emissionMask;
		//vec3 result = ambient + diffuse + specular + emission;

		float distance = length(u_light.position - fragPos);
		float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance));


		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		diffuse *= intensity;
		specular *= intensity;

		result = ambient + diffuse + specular;
	}
	else {
		result = vec3(u_light.ambient * vec3(texture(u_material.diffuse, texCoord)));
	}



	FragColor = vec4(result, 1.0);
}
