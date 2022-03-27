#version 330 core
#define normalise normalize
const uint type_directional = 0U;
const uint type_point = 1U;
const uint type_spot = 2U;

out vec4 FragCol;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	uint type;
	vec4 position;
	vec4 direction;
	float cutoff;
	float sharpness;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float linear;
	float quadratic;
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

vec3 PhongShading(vec3 diffuseTex, vec3 normal, vec3 lightDir, float intensity)
{
	// Ambient
	vec3 ambient = light.ambient * diffuseTex;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseTex * intensity;
	// Specular
	vec3 viewDir = normalise(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)) * intensity;

	return ambient + diffuse + specular;
}

void main()
{
	// Texture blending
	//vec4 texCol = mix(mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.6), texture(texture2, TexCoord), 0.1);
	vec3 diffuseTex = vec3(texture(material.diffuse, TexCoord));

	// Muli-use
	vec3 normal = normalise(Normal);
	vec3 lightDir;
	float attenuation = 1;

	if (light.type == type_directional) {
		lightDir = normalise(light.direction.xyz);
	} else {
		vec3 lightDiff = light.position.xyz - FragPos;
		lightDir = normalise(lightDiff);
		// Light fading over distance
		float lightDist = length(lightDiff);
		attenuation = 1.0 / (1.0 + light.linear * lightDist + light.quadratic * (lightDist * lightDist));
	}

	vec3 phong;

	if (light.type == type_spot) {
		float theta = dot(lightDir, normalise(light.direction.xyz));
		//float epsilon = light.cutoff - light.sharpness;
		//float intensity = clamp((theta - light.sharpness) / epsilon, 0.0, 1.0);
		float intensity = 1;
		if (theta > light.cutoff) {
			phong = PhongShading(diffuseTex, normal, lightDir, intensity);
		} else {
			FragCol = vec4(light.ambient * diffuseTex, 1.0);
			return;
		}
	} else {
		phong = PhongShading(diffuseTex, normal, lightDir, 1);
	}

	FragCol = vec4(phong * attenuation, 1.0);

	// // Ambient
	// vec3 ambient = light.ambient * diffuseTex;
	// // Diffuse
	// float diff = max(dot(normal, lightDir), 0.0);
	// vec3 diffuse = light.diffuse * diff * diffuseTex;
	// // Specular
	// vec3 viewDir = normalise(viewPos - FragPos);
	// vec3 reflectDir = reflect(-lightDir, normal);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	// vec3 phong = (ambient + diffuse + specular) * attenuation;

	// //FragCol = texCol * vec4(phong, 1.0);
	// FragCol = vec4(phong, 1.0);
}
