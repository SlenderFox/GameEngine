#pragma region
#include "Shader.hpp"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

using std::string;
using std::stringstream;
using std::ifstream;
#pragma endregion

namespace Engine
{
	// Forward declaration
	class Application { public: static const bool GladLoaded() noexcept; };

	Shader::Shader(string pShaderPath)
	{
		Load(pShaderPath);
	}

	Shader::~Shader()
	{
		if (Application::GladLoaded() && m_shaderLoaded)
			glDeleteProgram(m_idProgram);
	}

	void Shader::Use() const noexcept
	{
		glUseProgram(m_idProgram);
	}

	void Shader::Load(string pShaderPath)
	{
		m_shaderPath = pShaderPath;
		LoadShader(ShaderType::VERTEX);
		LoadShader(ShaderType::FRAGMENT);
		CreateShaderProgram();
	}

	void Shader::LoadShader(ShaderType pType)
	{
		m_usingFallback = false;
		string path = m_shaderPath + GetType(pType, string(".vert"), string(".frag"));
		#ifdef _DEBUG
		 cout << "Compiling shader \"" << path << "\"...";
		#endif

		if (pType == ShaderType::PROGRAM)
		{
			#ifdef _DEBUG
			 cout << "\nERROR::SHADER::ATTEMPTING_TO_LOAD_PROGRAM" << endl;
			#endif
			return;
		}

		if (std::strcmp(m_shaderPath.c_str(), "") == 0)
		{
			m_usingFallback = true;
		}

		#pragma region Fallback code
		 const char* vertexFallback = "#version 330 core\n\
layout(location=0)in vec3 aPos;\
layout(location=1)in vec3 aNormal;\
layout(location=2)in vec2 aTexCoords;\
out vec3 FragPos;\
out vec3 Normal;\
out vec2 TexCoords;\
uniform vec3 u_scale=vec3(1.0);\
uniform mat4 u_camera;\
uniform mat4 u_model;\
uniform mat3 u_transposeInverseOfModel;\
void main(){\
vec4 vertModel=u_model*vec4(aPos*u_scale,1.0);\
TexCoords=aTexCoords;\
Normal=u_transposeInverseOfModel*aNormal;\
FragPos=vec3(vertModel);\
gl_Position=u_camera*vertModel;}";
		 const char* fragmentFallback = "#version 330 core\n\
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
vec3 PhongShading(LightColour pColour,vec3 pLightDir,float pIntensity){\
vec3 diffuseTex=texture(u_material.texture_diffuse0,TexCoords).rgb;\
vec3 specularTex=texture(u_material.texture_specular0,TexCoords).rgb;\
float diff=max(dot(m_normal,pLightDir),0.0);\
vec3 reflectDir=reflect(-pLightDir,m_normal);\
float spec=pow(max(dot(m_viewDir,reflectDir),0.0),u_material.shininess);\
vec3 ambient=pColour.ambient*diffuseTex;\
vec3 diffuse=pColour.diffuse*diffuseTex*diff*pIntensity;\
vec3 specular=pColour.specular*specularTex*spec*pIntensity;\
return ambient+diffuse+specular;}\
float CalculateAttentuation(float pDist,float pLinear,float pQuadratic){\
return 1.0/(1.0+pLinear*pDist+pQuadratic*(pDist*pDist));}\
vec3 CalculateDirectionalLighting(LightDirectional pLight){\
vec3 lightDir=normalise(pLight.direction.xyz);\
return PhongShading(pLight.colour,lightDir,1);}\
vec3 CalculatePointLight(LightPoint pLight){\
if(length(pLight.linear)==0)return vec3(0);\
vec3 lightDiff=pLight.position.xyz-FragPos;\
vec3 lightDir=normalise(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist,pLight.linear,pLight.quadratic);\
return PhongShading(pLight.colour,lightDir,1)*attenuation;}\
vec3 CalculateSpotLight(LightSpot pLight){\
if(length(pLight.linear)==0)return vec3(0);\
vec3 lightDiff=pLight.position.xyz-FragPos;\
vec3 lightDir=normalise(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist,pLight.linear,pLight.quadratic);\
float intensity=1;\
float theta=dot(lightDir,normalise(pLight.direction.xyz));\
float epsilon=(pLight.blur*(1-pLight.cutoff)+pLight.cutoff)-pLight.cutoff;\
intensity=clamp((theta-pLight.cutoff)/epsilon,0.0,1.0);\
return PhongShading(pLight.colour,lightDir,intensity)*attenuation;}\
void main(){\
m_normal=normalise(Normal);\
m_viewDir=normalise(u_viewPos-FragPos);\
vec3 result;\
for(int i=0;i<NR_DIR_LIGHTS;++i)result+=CalculateDirectionalLighting(u_dirLights[i]);\
for(int i=0;i<NR_POINT_LIGHTS;++i)result+=CalculatePointLight(u_pointLights[i]);\
for(int i=0;i<NR_SPOT_LIGHTS;++i)result+=CalculateSpotLight(u_spotLights[i]);\
if(u_justColour) FragCol=vec4(u_colour,1);\
else FragCol=vec4(result*u_colour,1);\
return;}";
		#pragma endregion

		const char* shaderCode;
		// Must be defined outside the try catch
		string codeString;

		// Try to retrieve the vertex/fragment source code from filePath
		try
		{
			ifstream fileStream;
			stringstream codeStream;
			// Ensure ifstream objects can throw exceptions
			fileStream.exceptions(ifstream::failbit | ifstream::badbit);

			fileStream.open(path);
			codeStream << fileStream.rdbuf();
			fileStream.close();

			// Convert stream into string, has to be this dumb way
			codeString = codeStream.str();
			shaderCode = codeString.c_str();
		}
		catch (ifstream::failure e)
		{
			#ifdef _DEBUG
			 cout << "\nERROR::SHADER::" << GetType(pType, string("VERTEX"), string("FRAGMENT"))
			  << "::FAILURE_TO_READ_FILE::USING_FALLBACK_CODE" << endl;
			#endif

			m_usingFallback = true;
			shaderCode = GetType(pType, vertexFallback, fragmentFallback);
		}

		if (!m_usingFallback)
		{
			if (!CompileShader(GetType(pType, &m_idVertex, &m_idFragment), pType, shaderCode))
				m_usingFallback = true;
		}

		// Separated to allow bool to potentially change
		if (m_usingFallback)
		{
			#ifdef _DEBUG
			cout << "Compiling fallback code...";
			#endif
			if (!CompileShader(GetType(pType, &m_idVertex, &m_idFragment),
			 pType, GetType(pType, vertexFallback, fragmentFallback)))
			{
				#ifdef _DEBUG
				cout << "ERROR::SHADER::" << GetType(pType, string("VERTEX"), string("FRAGMENT"))
				 << "::FALLBACK_CODE_FAILURE" << endl;
				#endif
				exit(2);
			}
		}

		#ifdef _DEBUG
		 cout << "Success!" << endl;
		#endif
	}

	bool Shader::CompileShader(uint32_t* const& pId, ShaderType pType, const char* pCode)
	{
		// Creates a shader object and assigns to an id
		switch (pType)
		{
		case ShaderType::PROGRAM:
			#ifdef _DEBUG
			 printf("\nERROR::SHADER::ATTEMPTING_TO_COMPILE_PROGRAM");
			#endif
			exit(2);
		case ShaderType::VERTEX:
			*pId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::FRAGMENT:
			*pId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			#ifdef _DEBUG
			 printf("\nERROR::SHADER::UNKNOWN_SHADER_TYPE");
			#endif
			exit(2);
		}

		// Loads the shader code into the shader object
		glShaderSource(*pId, 1, &pCode, NULL);
		// Compiles the shader at run-time
		glCompileShader(*pId);
		// Performs error checking on the shader
		return CheckForErrors(pId, pType);
	}

	void Shader::CreateShaderProgram()
	{
		// Creates a shader program object assigned to id, this sets it as the active shader
		m_idProgram = glCreateProgram();
		// Link the vertex and fragment shaders
		glAttachShader(m_idProgram, m_idVertex);
		glAttachShader(m_idProgram, m_idFragment);
		glLinkProgram(m_idProgram);
		// Performs error checking on the shader program
		if (!CheckForErrors(&m_idProgram, ShaderType::PROGRAM)) exit(2);
		// We no longer need the vertex and fragment shaders
		glDeleteShader(m_idVertex);
		glDeleteShader(m_idFragment);
		// Sets the shader as the active one
		glUseProgram(m_idProgram);
		m_shaderLoaded = true;
	}
		
	bool Shader::CheckForErrors(const uint32_t* const& pShaderID, ShaderType pType) noexcept
	{
		// Boolean output as int32
		int32_t success;
		char infoLog[512];

		if (pType == ShaderType::PROGRAM)
		{
			// Retrieves the compile status of the given shader by id
			glGetProgramiv(*pShaderID, GL_LINK_STATUS, &success);
			if (!success)
			{
				// In the case of a failure it loads the log and outputs
				glGetProgramInfoLog(*pShaderID, 512, NULL, infoLog);
				#ifdef _DEBUG
				 cout << "\nERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << infoLog;
				#endif
				return false;
			}
		}
		else
		{
			// Retrieves the compile status of the given shader by id
			glGetShaderiv(*pShaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				// In the case of a failure it loads the log and outputs
				glGetShaderInfoLog(*pShaderID, 512, NULL, infoLog);
				#ifdef _DEBUG
				 cout << "\nERROR::SHADER::" << GetType(pType, string("VERTEX"), string("FRAGMENT"))
				  << "::COMPILATION_FAILED:\n" << infoLog;
				#endif
				return false;
			}
		}
		return true;
	}

	template<typename T>
	constexpr T Shader::GetType(ShaderType pType, T ifVertex, T ifFragment) const
	{
		if (pType == ShaderType::PROGRAM) exit(2);
		return (pType == ShaderType::VERTEX ? ifVertex : ifFragment);
	}

	#pragma region Setters
	void Shader::SetBool(const string& pName, const bool& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), (int32_t)pValue);
	}

	void Shader::SetInt(const string& pName, const int32_t& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetUint(const string& pName, const uint32_t& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1ui(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetFloat(const string& pName, const float& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1f(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetVec2(const string& pName, const glm::vec2& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform2fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec3(const string& pName, const glm::vec3& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec4(const string& pName, const glm::vec4& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetMat3(const string& pName, const glm::mat3& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniformMatrix3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, &pValue[0][0]);
	}

	void Shader::SetMat4(const string& pName, const glm::mat4& pValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniformMatrix4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, &pValue[0][0]);
	}
	#pragma endregion
}
