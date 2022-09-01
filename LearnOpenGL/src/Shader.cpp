#pragma region
#include "Shader.hpp"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

using std::stringstream;
using std::ifstream;
#pragma endregion

namespace Engine
{
	Shader::Shader()
	{
		//LoadVertexShader();
		//LoadFragmentShader();
		LoadShader(ShaderType::VERTEX);
		LoadShader(ShaderType::FRAGMENT);
		CreateShaderProgram();
	}

	Shader::Shader(string pShaderPath)
	{
		LoadPaths(pShaderPath);
	}

	void Shader::Destroy(bool pValidate)
	{
		if (pValidate && m_shaderLoaded)
			glDeleteProgram(m_idProgram);
	}

	void Shader::Use() const
	{
		glUseProgram(m_idProgram);
	}

	void Shader::LoadPaths(string pShaderPath)
	{
		m_shaderPath = pShaderPath;

		//LoadVertexShader();
		//LoadFragmentShadepShaderPathr();
		LoadShader(ShaderType::VERTEX);
		LoadShader(ShaderType::FRAGMENT);
		CreateShaderProgram();
	}

	void Shader::LoadShader(ShaderType pType)
	{
		if (pType == ShaderType::PROGRAM)
		{
			#ifdef _DEBUG
			 printf("ERROR::SHADER::ATTEMPTING_TO_LOAD_PROGRAM");
			#endif
			exit(0);
		}

		#pragma region Fallback code
		 const char* vertexFallback = "#version 330 core\n\
layout (location=0) in vec3 aPos;\
layout (location=1) in vec3 aNormal;\
layout (location=2) in vec2 aTexCoord;\
out vec3 FragPos;\
out vec3 Normal;\
out vec2 TexCoord;\
uniform mat4 u_camera;\
uniform mat4 u_model;\
uniform mat3 u_transposeInverseOfModel;\
void main(){\
gl_Position=u_camera*u_model*vec4(aPos,1.0);\
FragPos=vec3(u_model*vec4(aPos,1.0));\
Normal=u_transposeInverseOfModel*aNormal;\
TexCoord=aTexCoord;}";
		 const char* fragmentFallback = "#version 330 core\n\
#define normalise normalize\
const int NR_POINT_LIGHTS=10;\
const int NR_SPOT_LIGHTS=10;\
out vec4 FragCol;\
in vec3 FragPos;\
in vec3 Normal;\
in vec2 TexCoord;\
struct Material {sampler2D texture_diffuse0;sampler2D texture_specular0;float shininess;};\
struct Colour {vec3 ambient;vec3 diffuse;vec3 specular;};\
struct LightDirectional {Colour colour;vec4 direction;};\
struct LightPoint {Colour colour;vec4 position;float linear;float quadratic;};\
struct LightSpot {Colour colour;vec4 position;vec4 direction;float linear;float quadratic;float cutoff;float blur;};\
uniform vec3 u_viewPos;\
uniform Material u_material;\
uniform LightDirectional u_directional;\
uniform LightPoint[NR_POINT_LIGHTS] u_pointLights;\
uniform LightSpot[NR_SPOT_LIGHTS] u_spotLights;\
vec3 m_normal;\
vec3 m_viewDir;\
vec3 PhongShading(Colour pColour,vec3 pLightDir,float pIntensity){\
vec3 diffuseTex=texture(u_material.texture_diffuse0,TexCoord).rgb;\
vec3 specularTex=texture(u_material.texture_specular0,TexCoord).rgb;\
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
vec3 lightDir=normalize(u_directional.direction.xyz);\
return PhongShading(pLight.colour,lightDir,1);}\
vec3 CalculatePointLight(LightPoint pLight){\
if (length(pLight.linear)==0)return vec3(0);\
vec3 lightDiff=pLight.position.xyz-FragPos;\
vec3 lightDir=normalize(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist,pLight.linear,pLight.quadratic);\
return PhongShading(pLight.colour,lightDir,1)*attenuation;}\
vec3 CalculateSpotLight(LightSpot pLight){\
if (length(pLight.linear)==0)return vec3(0);\
vec3 lightDiff=pLight.position.xyz-FragPos;\
vec3 lightDir=normalize(lightDiff);\
float lightDist=length(lightDiff);\
float attenuation=CalculateAttentuation(lightDist, pLight.linear, pLight.quadratic);\
float intensity=1;\
float theta=dot(lightDir,normalize(pLight.direction.xyz));\
float epsilon=(pLight.blur*(1-pLight.cutoff)+pLight.cutoff)-pLight.cutoff;\
intensity=clamp((theta-pLight.cutoff)/epsilon,0.0,1.0);\
return PhongShading(pLight.colour,lightDir,intensity)*attenuation;}\
void main(){\
m_normal=normalize(Normal);\
m_viewDir=normalize(u_viewPos-FragPos);\
vec3 result=CalculateDirectionalLighting(u_directional);\
for(int i=0;i<NR_POINT_LIGHTS;++i)result+=CalculatePointLight(u_pointLights[i]);\
for(int i=0;i<NR_SPOT_LIGHTS;++i)result+=CalculateSpotLight(u_spotLights[i]);\
FragCol=vec4(result,1);return;}";
		#pragma endregion

		// Must be defined out here
		const char* code;
		ifstream inStream;
		string codeString;

		// Ensure ifstream objects can throw exceptions
		inStream.exceptions(ifstream::failbit | ifstream::badbit);

		// 1. retrieve the vertex/fragment source code from filePath
		try
		{
			/* For both vertex and fragment we:
			 * Open files
			 * Read file's buffer contents into streams
			 * Close file handlers
			 * Convert stream into string
			*/
			stringstream codeStream;
			inStream.open((pType == ShaderType::VERTEX ? m_shaderPath + string(".vert") : m_shaderPath + string(".frag")));
			codeStream << inStream.rdbuf();
			inStream.close();
			codeString = codeStream.str();
			code = codeString.c_str();
		}
		catch (ifstream::failure e)
		{
			#ifdef _DEBUG
			 cout << "ERROR::SHADER::" << (pType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "::FILE_NOT_SUCCESFULLY_READ::USING_FALLBACK" << endl;
			#endif

			code = (pType == ShaderType::VERTEX ? vertexFallback : fragmentFallback);
		}

		// 2. compile shaders
		if (!CompileShader((pType == ShaderType::VERTEX ? &m_idVertex : &m_idFragment), pType, code))
		{
			#ifdef _DEBUG
			 cout << "ERROR::SHADER::" << (pType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "::USING_FALLBACK_CODE\n" << endl;
			#endif
			if (!CompileShader((pType == ShaderType::VERTEX ? &m_idVertex : &m_idFragment), pType, vertexFallback))
			{
				#ifdef _DEBUG
			 	 cout << "ERROR::SHADER::" << (pType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "::FALLBACK_CODE_FAILURE\n" << endl;
				#endif
				exit(0);
			}
		}
	}

	bool Shader::CompileShader(unsigned int* pId, ShaderType pType, const char* pCode)
	{
		// Creates a shader object and assigns to an id
		if (pType == ShaderType::PROGRAM)
		{
			#ifdef _DEBUG
			 printf("ERROR::SHADER::ATTEMPTING_TO_COMPILE_PROGRAM");
			#endif
			exit(0);
		}
		else if (pType == ShaderType::VERTEX)
			*pId = glCreateShader(GL_VERTEX_SHADER);
		else if (pType == ShaderType::FRAGMENT)
			*pId = glCreateShader(GL_FRAGMENT_SHADER);

		// Loads the vertex shader into the object
		glShaderSource(*pId, 1, &pCode, NULL);
		// Compiles the shader at run-time
		glCompileShader(*pId);
		// Performs error checking on the vertex shader
		return ShaderErrorChecking(pId, pType);
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
		ShaderErrorChecking(&m_idProgram, ShaderType::PROGRAM);
		// We no longer need the vertex and fragment shaders
		glDeleteShader(m_idVertex);
		glDeleteShader(m_idFragment);

		// Sets the shader as the active one
		glUseProgram(m_idProgram);

		m_shaderLoaded = true;
	}
		
	bool Shader::ShaderErrorChecking(unsigned int* pShaderID, ShaderType pType)
	{
		// Variables used in error checking and handling
		int success;
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
				 cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
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
				 cout << "ERROR::SHADER::" << (pType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << endl;
				#endif
				return false;
			}
		}
		return true;
	}

	#pragma region Setters
	void Shader::SetBool(const string& pName, bool pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), (int)pValue);
	}

	void Shader::SetInt(const string& pName, int pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetUint(const string& pName, unsigned int pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform1ui(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetFloat(const string& pName, float pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform1f(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetVec2(const string& pName, float pX, float pY) const
	{
		glUseProgram(m_idProgram);
		glUniform2f(glGetUniformLocation(m_idProgram, pName.c_str()), pX, pY);
	}

	void Shader::SetVec2(const string& pName, glm::vec2 pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform2fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec2(const string& pName, glm::vec2& pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform2fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec3(const string& pName, float pX, float pY, float pZ) const
	{
		glUseProgram(m_idProgram);
		glUniform3f(glGetUniformLocation(m_idProgram, pName.c_str()), pX, pY, pZ);
	}

	void Shader::SetVec3(const string& pName, vec3 pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec3(const string& pName, vec3& pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec4(const string& pName, float pX, float pY, float pZ, float pW) const
	{
		glUseProgram(m_idProgram);
		glUniform4f(glGetUniformLocation(m_idProgram, pName.c_str()), pX, pY, pZ, pW);
	}

	void Shader::SetVec4(const string& pName, glm::vec4 pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetVec4(const string& pName, glm::vec4& pValue) const
	{
		glUseProgram(m_idProgram);
		glUniform4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, &pValue[0]);
	}

	void Shader::SetMat3(const string& pName, glm::mat3 pValue) const
	{
		glUseProgram(m_idProgram);
		glUniformMatrix3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, glm::value_ptr(pValue));
	}

	void Shader::SetMat3(const string& pName, glm::mat3& pValue) const
	{
		glUseProgram(m_idProgram);
		glUniformMatrix3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, &pValue[0][0]);
	}

	void Shader::SetMat4(const string& pName, glm::mat4 pValue) const
	{
		glUseProgram(m_idProgram);
		glUniformMatrix4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, glm::value_ptr(pValue));
	}

	void Shader::SetMat4(const string& pName, glm::mat4& pValue) const
	{
		glUseProgram(m_idProgram);
		glUniformMatrix4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, &pValue[0][0]);
	}
	#pragma endregion
}
