#pragma once

#define VERTEX_FALLBACK "#version 330 core\n\
layout(location=0)in vec3 aPos;\
layout(location=1)in vec3 aNormal;\
layout(location=2)in vec2 aTexCoords;\
out vec3 FragPos;\
out vec3 Normal;\
out vec2 TexCoords;\
uniform mat4 u_camera;\
uniform mat4 u_model;\
uniform mat3 u_transposeInverseOfModel;\
void main(){\
vec4 vertModel=u_model*vec4(aPos,1.0);\
TexCoords=aTexCoords;\
Normal=u_transposeInverseOfModel*aNormal;\
FragPos=vec3(vertModel);\
gl_Position=u_camera*vertModel;}"

#define FRAGMENT_FALLBACK "#version 330 core\n\
#define normalise normalize\n\
const int NR_DIR_LIGHTS=3;\
const int NR_POINT_LIGHTS=30;\
const int NR_SPOT_LIGHTS=30;\
const float near=0.1;\
const float far=500.0;\
out vec4 FragCol;\
in vec3 FragPos;\
in vec3 Normal;\
in vec2 TexCoords;\
struct Material{float shininess;sampler2D texture_diffuse0;sampler2D texture_specular0;};\
struct LightColour{vec3 ambient;vec3 diffuse;vec3 specular;};\
struct LightDirectional{LightColour colour;vec4 direction;};\
struct LightPoint{LightColour colour;vec4 position;float linear;float quadratic;};\
struct LightSpot{LightColour colour;vec4 position;vec4 direction;float linear;float quadratic;float cutoff;float blur;};\
uniform bool u_justColour=false;\
uniform vec3 u_viewPos;\
uniform vec3 u_colour=vec3(1.0);\
uniform Material u_material;\
uniform LightDirectional[NR_DIR_LIGHTS] u_dirLights;\
uniform LightPoint[NR_POINT_LIGHTS] u_pointLights;\
uniform LightSpot[NR_SPOT_LIGHTS] u_spotLights;\
vec3 m_normal;\
vec3 m_viewDir;\
float LineariseDepth(float pDepth){\
float z=pDepth*2.0-1.0;\
return (2.0*near*far)/(far+near-z*(far-near));}\
vec3 PhongShading(LightColour _colour,vec3 _lightDir,float _intensity){\
vec3 diffuseTex=texture(u_material.texture_diffuse0,TexCoords).rgb;\
vec3 specularTex=texture(u_material.texture_specular0,TexCoords).rgb;\
float diff=max(dot(m_normal,_lightDir),0.0);\
vec3 reflectDir=reflect(-_lightDir,m_normal);\
float spec=pow(max(dot(m_viewDir,reflectDir),0.0),u_material.shininess);\
vec3 ambient=_colour.ambient*diffuseTex;\
vec3 diffuse=_colour.diffuse*diffuseTex*diff*_intensity;\
vec3 specular=_colour.specular*specularTex*spec*_intensity;\
return ambient+diffuse+specular;}\
float CalculateAttentuation(float _dist,float _linear,float _quadratic){\
return 1.0/(1.0+_linear*_dist+_quadratic*(_dist*_dist));}\
vec3 CalculateDirectionalLighting(LightDirectional _light){\
vec3 lightDir=normalise(_light.direction.xyz);\
return PhongShading(_light.colour,lightDir,1);}\
vec3 CalculatePointLight(LightPoint _light){\
if(length(_light.linear)==0)return vec3(0);\
vec3 lightDiff=_light.position.xyz-FragPos;\
vec3 lightDir=normalise(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist,_light.linear,_light.quadratic);\
return PhongShading(_light.colour,lightDir,1)*attenuation;}\
vec3 CalculateSpotLight(LightSpot _light){\
if(length(_light.linear)==0)return vec3(0);\
vec3 lightDiff=_light.position.xyz-FragPos;\
vec3 lightDir=normalise(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist,_light.linear,_light.quadratic);\
float intensity=1;\
float theta=dot(lightDir,normalise(_light.direction.xyz));\
float epsilon=(_light.blur*(1-_light.cutoff)+_light.cutoff)-_light.cutoff;\
intensity=clamp((theta-_light.cutoff)/epsilon,0.0,1.0);\
return PhongShading(_light.colour,lightDir,intensity)*attenuation;}\
void main(){\
m_normal=normalise(Normal);\
m_viewDir=normalise(u_viewPos-FragPos);\
vec3 result;\
for(int i=0;i<NR_DIR_LIGHTS;++i)result+=CalculateDirectionalLighting(u_dirLights[i]);\
for(int i=0;i<NR_POINT_LIGHTS;++i)result+=CalculatePointLight(u_pointLights[i]);\
for(int i=0;i<NR_SPOT_LIGHTS;++i)result+=CalculateSpotLight(u_spotLights[i]);\
if(u_justColour) FragCol=vec4(u_colour,1);\
else FragCol=vec4(result*u_colour,1);\
return;}"
