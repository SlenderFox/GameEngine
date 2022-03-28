#version 330 core
#define normalise normalize
// Light types
const uint type_directional = 0U;
const uint type_point = 1U;
const uint type_spot = 2U;

// I/O
out vec4 FragCol;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

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
	float blur;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 PhongShading(vec3 lightDir, float intensity)
{
	vec3 diffuseTex = texture(material.diffuse, TexCoord).rgb;
	vec3 normal = normalise(Normal);
	// Ambient
	vec3 ambient = light.ambient * diffuseTex;
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseTex * intensity;
	// Specular
	vec3 viewDir = normalise(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb * intensity;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 lightDir;
	float attenuation = 1;

	if (light.type == type_directional) {
		lightDir = normalise(light.direction.xyz);
	} else {
		// Point or spotlight
		vec3 lightDiff = light.position.xyz - FragPos;
		lightDir = normalise(lightDiff);
		// Light fading over distance
		float lightDist = length(lightDiff);
		attenuation = 1.0 / (1.0 + light.linear * lightDist + light.quadratic * (lightDist * lightDist));
	}

	float intensity = 1;
	// Soft edges
	if (light.type == type_spot) {
		float theta = dot(lightDir, normalise(light.direction.xyz));
		//float epsilon = light.cutoff - light.blur;
		//intensity = clamp((theta - light.blur) / epsilon, 0.0, 1.0);
		// l(1-c)+c scales light.blur from 0-1 to light.cutoff-1
		float epsilon = (light.blur * (1 - light.cutoff) + light.cutoff) - light.cutoff;
		intensity = clamp((theta - light.cutoff) / epsilon, 0.0, 1.0);

		if (TexCoord.x < 0.02 || TexCoord.y < 0.02 || TexCoord.x > 0.98 || TexCoord.y > 0.98) {
			FragCol = vec4(intensity, intensity, intensity, 1);
			return;
		}
	}

	vec3 phong = PhongShading(lightDir, intensity) * attenuation;
	FragCol = vec4(phong, 1.0);
}
