#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 fragPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;

    float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material u_material;
uniform DirLight u_dirLight;
uniform vec3 u_viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    u_dirLight;
    u_dirLight.direction;

    u_material.texture_diffuse1;
    u_material.texture_diffuse2;
    u_material.texture_diffuse3;
    u_material.texture_specular1;
    u_material.texture_specular2;


    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(u_viewPos - fragPos);

    vec3 result = CalcDirLight(u_dirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	vec3 ambient = light.ambient * vec3(texture(u_material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(u_material.texture_specular1, TexCoords));

	return (ambient + diffuse + specular);
}