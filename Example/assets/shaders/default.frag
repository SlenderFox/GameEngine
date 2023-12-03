#version 330 core
#define normalise normalize
const int NR_DIR_LIGHTS=3;
const int NR_POINT_LIGHTS=30;
const int NR_SPOT_LIGHTS=30;
const float near=0.1;
const float far=500.0;
// I/O
out vec4 FragCol;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
struct Material{
	float shininess;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
};
struct LightColour{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct LightDirectional{
	LightColour colour;
	vec4 direction;
};
struct LightPoint{
	LightColour colour;
	vec4 position;
	float linear;
	float quadratic;
};
struct LightSpot{
	LightColour colour;
	vec4 position;
	vec4 direction;
	float linear;
	float quadratic;
	float cutoff;
	float blur;
};
uniform bool u_depthBuffer=false;
uniform bool u_justColour=false;
uniform vec3 u_viewPos;
uniform vec3 u_colour=vec3(1.0);
uniform Material u_material;
uniform LightDirectional[NR_DIR_LIGHTS] u_dirLights;
uniform LightPoint[NR_POINT_LIGHTS] u_pointLights;
uniform LightSpot[NR_SPOT_LIGHTS] u_spotLights;
vec3 m_normal;
vec3 m_viewDir;
vec3 PhongShading(LightColour _colour,vec3 _lightDir,float _intensity){
	// Textures
	vec3 diffuseTex=texture(u_material.texture_diffuse0,TexCoords).rgb;
	vec3 specularTex=texture(u_material.texture_specular0,TexCoords).rgb;
	// Diffuse shading
	float diff=max(dot(m_normal,_lightDir),0.0);
	// Specular shading
	vec3 reflectDir=reflect(-_lightDir,m_normal);
	float spec=pow(max(dot(m_viewDir,reflectDir),0.0),u_material.shininess);
	// Combine results
	vec3 ambient=_colour.ambient*diffuseTex;
	vec3 diffuse=_colour.diffuse*diffuseTex*diff*_intensity;
	vec3 specular=_colour.specular*specularTex*spec*_intensity;
	return ambient+diffuse+specular;
}
float CalculateAttentuation(float _dist,float _linear,float _quadratic){
	return 1.0/(1.0+_linear*_dist+_quadratic*(_dist*_dist));
}
vec3 CalculateDirectionalLighting(LightDirectional _light){
	vec3 lightDir=normalise(_light.direction.xyz);
	return PhongShading(_light.colour,lightDir,1);
}
vec3 CalculatePointLight(LightPoint _light){
	// Skip over "dead" lights
	if(length(_light.linear)==0)
		return vec3(0);
	// Point or spotlight
	vec3 lightDiff=_light.position.xyz-FragPos;
	vec3 lightDir=normalise(lightDiff);
	// Light fading over distance
	float lightDist=length(lightDiff);
	float attenuation=CalculateAttentuation(lightDist,_light.linear,_light.quadratic);
	return PhongShading(_light.colour,lightDir,1)*attenuation;
}
vec3 CalculateSpotLight(LightSpot _light){
	// Skip over "dead" lights
	if(length(_light.linear)==0)
		return vec3(0);
	// Point or spotlight
	vec3 lightDiff=_light.position.xyz-FragPos;
	vec3 lightDir=normalise(lightDiff);
	// Light fading over distance
	float lightDist=length(lightDiff);
	float attenuation=CalculateAttentuation(lightDist,_light.linear,_light.quadratic);
	// Soft edges
	float intensity=1;
	float theta=dot(lightDir,normalise(_light.direction.xyz));
	// l(1-c)+c scales light.blur from 0-1 to light.cutoff-1
	float epsilon=(_light.blur*(1-_light.cutoff)+_light.cutoff)-_light.cutoff;
	intensity=clamp((theta-_light.cutoff)/epsilon,0.0,1.0);
	return PhongShading(_light.colour,lightDir,intensity)*attenuation;
}
float LineariseDepth(float pDepth){
	float z=pDepth*2.0-1.0;	// Back to ndc
	return (2.0*near*far)/(far+near-z*(far-near));
}
void main(){
	if (u_depthBuffer){
		//FragCol=vec4(vec3(gl_FragCoord.z),1.0);
		FragCol=vec4(vec3(LineariseDepth(gl_FragCoord.z)/far),1.0);
	}else if(u_justColour){
		FragCol=vec4(u_colour,1);
	}else{
		// Important vectors
		m_normal=normalise(Normal);
		m_viewDir=normalise(u_viewPos-FragPos);
		vec3 result;
		for(int i=0;i<NR_DIR_LIGHTS;++i)
			result+=CalculateDirectionalLighting(u_dirLights[i]);
		for(int i=0;i<NR_POINT_LIGHTS;++i)
			result+=CalculatePointLight(u_pointLights[i]);
		for(int i=0;i<NR_SPOT_LIGHTS;++i)
			result+=CalculateSpotLight(u_spotLights[i]);
		FragCol=vec4(result*u_colour,1);
	}
	return;
}
