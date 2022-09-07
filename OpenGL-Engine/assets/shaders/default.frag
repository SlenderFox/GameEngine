#version 330 core
#define normalise normalize

const int NR_DIR_LIGHTS = 3;
const int NR_POINT_LIGHTS = 30;
const int NR_SPOT_LIGHTS = 30;

// I/O
out vec4 FragCol;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
	float shininess;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
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

uniform LightDirectional[NR_DIR_LIGHTS] u_dirLights;
uniform LightPoint[NR_POINT_LIGHTS] u_pointLights;
uniform LightSpot[NR_SPOT_LIGHTS] u_spotLights;

vec3 m_normal;
vec3 m_viewDir;

const float near = 0.1;
const float far = 500.0;

float LineariseDepth(float pDepth)
{
	float z = pDepth * 2.0 - 1.0; // Back to ndc
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 PhongShading(Colour pColour, vec3 pLightDir, float pIntensity)
{	
	//Textures
	vec3 diffuseTex = texture(u_material.texture_diffuse0, TexCoords).rgb;
	vec3 specularTex = texture(u_material.texture_specular0, TexCoords).rgb;
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
	vec3 lightDir = normalise(pLight.direction.xyz);
	return PhongShading(pLight.colour, lightDir, 1);
}

vec3 CalculatePointLight(LightPoint pLight)
{
	// Skip over "dead" lights
	if (length(pLight.linear) == 0)
		return vec3(0);

	// Point or spotlight
	vec3 lightDiff = pLight.position.xyz - FragPos;
	vec3 lightDir = normalise(lightDiff);
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
	vec3 lightDir = normalise(lightDiff);
	// Light fading over distance
	float lightDist = length(lightDiff);
	float attenuation = CalculateAttentuation(lightDist, pLight.linear, pLight.quadratic);

	// Soft edges
	float intensity = 1;
	float theta = dot(lightDir, normalise(pLight.direction.xyz));
	// l(1-c)+c scales light.blur from 0-1 to light.cutoff-1
	float epsilon = (pLight.blur * (1 - pLight.cutoff) + pLight.cutoff) - pLight.cutoff;
	intensity = clamp((theta - pLight.cutoff) / epsilon, 0.0, 1.0);

	return PhongShading(pLight.colour, lightDir, intensity) * attenuation;
}

void main()
{
	// Important vectors
	m_normal = normalise(Normal);
	m_viewDir = normalise(u_viewPos - FragPos);

	vec3 result;
	for (int i = 0; i < NR_DIR_LIGHTS; ++i)
		result += CalculateDirectionalLighting(u_dirLights[i]);

	for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalculatePointLight(u_pointLights[i]);

	for (int i = 0; i < NR_SPOT_LIGHTS; ++i)
		result += CalculateSpotLight(u_spotLights[i]);

	FragCol = vec4(result, 1);
	//FragCol = vec4(vec3(LineariseDepth(gl_FragCoord.z) / far), 1.0);
	return;
}
