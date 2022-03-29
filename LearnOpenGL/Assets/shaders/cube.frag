#version 330 core
#define normalise normalize

const int NR_POINT_LIGHTS = 10;
const int NR_SPOT_LIGHTS = 10;

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

struct Colour {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct LightDirectional {
	Colour colour;
	vec4 direction;
};

struct LightPoint {
	Colour colour;
	vec4 position;
	float linear;
	float quadratic;
};

struct LightSpot {
	Colour colour;
	vec4 position;
	vec4 direction;
	float linear;
	float quadratic;
	float cutoff;
	float blur;
};

uniform vec3 u_viewPos;
uniform Material u_material;

uniform LightDirectional u_directional;
uniform LightPoint[NR_POINT_LIGHTS] u_pointLights;
uniform LightSpot[NR_SPOT_LIGHTS] u_spotLights;

vec3 m_normal;
vec3 m_viewDir;

vec3 PhongShading(Colour pColour, vec3 pLightDir, float pIntensity)
{	
	//Textures
	vec3 diffuseTex = texture(u_material.diffuse, TexCoord).rgb;
	vec3 specularTex = texture(u_material.specular, TexCoord).rgb;
	// Diffuse shading
	float diff = max(dot(m_normal, pLightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-pLightDir, m_normal);
	float spec = pow(max(dot(m_viewDir, reflectDir), 0.0), u_material.shininess);
	// Combine results
	vec3 ambient = pColour.ambient * diffuseTex;
	vec3 diffuse = pColour.diffuse * diffuseTex * diff * pIntensity;
	vec3 specular = pColour.specular * specularTex * spec * pIntensity;
	return ambient + diffuse + specular;
}

float CalculateAttentuation(float pDist, float pLinear, float pQuadratic)
{
	return 1.0 / (1.0 + pLinear * pDist + pQuadratic * (pDist * pDist));
}

vec3 CalculateDirectionalLighting(LightDirectional pLight)
{
	vec3 lightDir = normalize(u_directional.direction.xyz);
	return PhongShading(pLight.colour, lightDir, 1);
}

vec3 CalculatePointLight(LightPoint pLight)
{
	// Skip over "dead" lights
	if (length(pLight.linear) == 0)
		return vec3(0);

	// Point or spotlight
	vec3 lightDiff = pLight.position.xyz - FragPos;
	vec3 lightDir = normalize(lightDiff);
	// Light fading over distance
	float lightDist = length(lightDiff);
	float attenuation = CalculateAttentuation(lightDist, pLight.linear, pLight.quadratic);

	return PhongShading(pLight.colour, lightDir, 1) * attenuation;
}

vec3 CalculateSpotLight(LightSpot pLight)
{
	// Skip over "dead" lights
	if (length(pLight.linear) == 0)
		return vec3(0);

	// Point or spotlight
	vec3 lightDiff = pLight.position.xyz - FragPos;
	vec3 lightDir = normalize(lightDiff);
	// Light fading over distance
	float lightDist = length(lightDiff);
	float attenuation = CalculateAttentuation(lightDist, pLight.linear, pLight.quadratic);

	// Soft edges
	float intensity = 1;
	float theta = dot(lightDir, normalize(pLight.direction.xyz));
	// l(1-c)+c scales light.blur from 0-1 to light.cutoff-1
	float epsilon = (pLight.blur * (1 - pLight.cutoff) + pLight.cutoff) - pLight.cutoff;
	intensity = clamp((theta - pLight.cutoff) / epsilon, 0.0, 1.0);

	return PhongShading(pLight.colour, lightDir, intensity) * attenuation;
}

void main()
{
	// Important vectors
	m_normal = normalize(Normal);
	m_viewDir = normalize(u_viewPos - FragPos);

	// Only one directional light allowed
	vec3 result = CalculateDirectionalLighting(u_directional);

	for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalculatePointLight(u_pointLights[i]);

	for (int i = 0; i < NR_SPOT_LIGHTS; ++i)
		result += CalculateSpotLight(u_spotLights[i]);

	FragCol = vec4(result, 1);
	return;
}
