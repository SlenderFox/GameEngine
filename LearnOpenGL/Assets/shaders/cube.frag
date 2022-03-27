#version 330 core
#define normalise normalize

out vec4 FragCol;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	//vec3 position;
	//vec3 direction;
	vec4 vector;	// Either the position or direction of the light

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
	// Texture blending
	//vec4 texCol = mix(mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.6), texture(texture2, TexCoord), 0.1);
	vec3 diffuseTex = vec3(texture(material.diffuse, TexCoord));

	// Important vectors
	vec3 norm = normalise(Normal);
	vec4 lightDir;
	if (light.vector.w < 0.0001)
		lightDir = normalise(-light.vector);
	else if (light.vector.w > 0.9999)
		lightDir = normalise(light.vector - vec4(FragPos, 1));
	// Ambient
	vec3 ambient = light.ambient * diffuseTex;
	// Diffuse
	float diff = max(dot(norm, vec3(lightDir)), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseTex;
	// Specular
	vec3 viewDir = normalise(viewPos - FragPos);
	vec3 reflectDir = reflect(-vec3(lightDir), norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 phong = ambient + diffuse + specular;

	//FragCol = texCol * vec4(phong, 1.0);
	FragCol = vec4(phong, 1.0);
}
