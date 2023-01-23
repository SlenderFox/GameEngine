#include "shader.hpp"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "debug.hpp"

using std::string;
using std::stringstream;
using std::ifstream;

namespace srender
{
	// Forward declaration
	class application { public: [[nodiscard]] static const bool gladLoaded() noexcept; };

	shader::shader(const string *inShaderPath)
	{ load(inShaderPath); }

	shader::~shader()
	{ destroy(); }

	void shader::destroy() noexcept
	{
		if (m_shaderLoaded && application::gladLoaded())
		{
			glDeleteProgram(m_idProgram);
			m_shaderLoaded = false;
		}
	}

	void shader::load(const std::string *inShaderPath)
	{
		if (m_shaderLoaded)
		{
			debug::send("ERROR::SHADER::ATTEMPTING_TO_LOAD_NEW_SHADER_WITHOUT_DELETING_OLD_ONE");
			return;
		}
		// If no path is given, will use fallback shader
		m_shaderPath = inShaderPath ? *inShaderPath : "";
		loadShader(shaderType::vertex);
		loadShader(shaderType::fragment);
		createShaderProgram();
	}

	void shader::use() const noexcept
	{ glUseProgram(m_idProgram); }

	constexpr bool shader::isLoaded() const noexcept
	{ return m_shaderLoaded; }

	inline void shader::loadShader(const shaderType inType)
	{
		const char *vertexFallback = "#version 330 core\n\
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
		const char *fragmentFallback = "#version 330 core\n\
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

		bool m_usingFallback = false;
		string codeString;

		if (inType == shaderType::program)
		{
			debug::send(
				"ERROR::SHADER::LOADING_INCORRECT_SHADER_TYPE",
				debug::type::Note,
				debug::impact::Large,
				debug::stage::Mid
			);
			return;
		}

		string path = m_shaderPath + byType(inType, string(".vert"), string(".frag"));
		debug::send(
			"Compiling shader \"" + path + "\"...",
			debug::type::Process,
			debug::impact::Small,
			debug::stage::Mid,
			false,
			false
		);

		if (m_shaderPath == "")
		{
			m_usingFallback = true;
			debug::newLine();
		}
		else
		{
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

				// Convert stream into string
				codeString = codeStream.str();
			}
			catch (ifstream::failure e)
			{
				string msg = "ERROR::SHADER::"
					+ byType(inType, string("VERTEX"), string("FRAGMENT"))
					+ "::FAILURE_TO_READ_FILE::USING_FALLBACK_CODE";
				debug::send(
					msg,
					debug::type::Note,
					debug::impact::Large,
					debug::stage::Mid,
					true
				);

				m_usingFallback = true;
			}
		}

		if (!m_usingFallback)
		{
			if (
				!compileShader(
					byType(inType, &m_idVertex, &m_idFragment),
					inType,
					codeString.c_str()
				)
			)
			{
				m_usingFallback = true;
			}
		}

		// Separated to allow bool to potentially change
		if (m_usingFallback)
		{
			debug::send(
				"Compiling fallback code...",
				debug::type::Process,
				debug::impact::Small,
				debug::stage::Mid,
				false,
				false
			);

			if (
				!compileShader(
					byType(inType, &m_idVertex, &m_idFragment),
					inType,
					byType<const char*&>(inType, vertexFallback, fragmentFallback)
				)
			)
			{
				string msg = "ERROR::SHADER::"
					+ byType(inType, string("VERTEX"), string("FRAGMENT"))
					+ "::FALLBACK_CODE_FAILURE";
				debug::send(
					msg,
					debug::type::Note,
					debug::impact::Large,
					debug::stage::Mid,
					true
				);

				exit(2);
			}
		}

		debug::send("Success!");
	}

	inline bool shader::compileShader(
		uint32_t *pId,
		shaderType inType,
		const char *inCode
	) noexcept
	{
		assert(inType != shaderType::program && "Incorrect shaderType passed");

		// Creates a shader object and assigns to an id
		switch (inType)
		{
		case shaderType::vertex:
			*pId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case shaderType::fragment:
			*pId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			debug::send(
				"ERROR::SHADER::ATTEMPTING_TO_COMPILE_UNKNOWN_SHADER_TYPE",
				debug::type::Note,
				debug::impact::Large,
				debug::stage::Mid,
				true
			);
			return false;
		}

		// Loads the shader code into the shader object
		glShaderSource(*pId, 1, &inCode, NULL);
		// Compiles the shader at run-time
		glCompileShader(*pId);
		// Performs error checking on the shader
		return checkForErrors(pId, inType);
	}

	inline void shader::createShaderProgram() noexcept
	{
		// Creates a shader program object assigned to id, this sets it as the active shader
		m_idProgram = glCreateProgram();
		// Link the vertex and fragment shaders
		glAttachShader(m_idProgram, m_idVertex);
		glAttachShader(m_idProgram, m_idFragment);
		glLinkProgram(m_idProgram);
		// Performs error checking on the shader program
		if (!checkForErrors(&m_idProgram, shaderType::program))
		{
			glDeleteProgram(m_idProgram);
			return;
		}
		// We no longer need the vertex and fragment shaders
		glDeleteShader(m_idVertex);
		glDeleteShader(m_idFragment);
		// Sets the shader as the active one
		glUseProgram(m_idProgram);
		m_shaderLoaded = true;
	}

	inline bool shader::checkForErrors(
		const uint32_t *inShaderID,
		const shaderType inType
	) const noexcept
	{
		// Boolean output as int32
		int32_t success;
		char infoLog[512];

		if (inType == shaderType::program)
		{
			// Retrieves the compile status of the given shader by id
			glGetProgramiv(*inShaderID, GL_LINK_STATUS, &success);
			if (!success)
			{
				// In the case of a failure it loads the log and outputs
				glGetProgramInfoLog(*inShaderID, 512, NULL, infoLog);
				debug::send(
					"ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" + string(infoLog),
					debug::type::Note,
					debug::impact::Large,
					debug::stage::Mid,
					true,
					false
				);
				return false;
			}
		}
		else
		{
			// Retrieves the compile status of the given shader by id
			glGetShaderiv(*inShaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				// In the case of a failure it loads the log and outputs
				glGetShaderInfoLog(*inShaderID, 512, NULL, infoLog);
				string msg = "ERROR::SHADER::"
					+ byType(inType, string("VERTEX"), string("FRAGMENT"))
					+ "::COMPILATION_FAILED:\n"
					+ string(infoLog);
				debug::send(
					msg,
					debug::type::Note,
					debug::impact::Large,
					debug::stage::Mid,
					true,
					false
				);
				return false;
			}
		}
		return true;
	}

	template<typename T> inline
	T shader::byType(
		const shaderType inType,
		T inVertex,
		T inFragment
	) const noexcept
	{
		assert(inType != shaderType::program && "Incorrect shaderType passed");
		return (inType == shaderType::vertex ? inVertex : inFragment);
	}

	void shader::setBool(string inName, bool inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1i(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			(int32_t)inValue
		);
	}

	void shader::setInt(string inName, int32_t inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1i(glGetUniformLocation(
			m_idProgram, inName.c_str()),
			inValue
		);
	}

	void shader::setUint(string inName, uint32_t inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1ui(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			inValue
		);
	}

	void shader::setFloat(string inName, float inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform1f(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			inValue
		);
	}

	void shader::setFloat2(string inName, glm::vec2 inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform2fv(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			1,
			&inValue[0]
		);
	}

	void shader::setFloat3(string inName, glm::vec3 inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform3fv(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			1,
			&inValue[0]
		);
	}

	void shader::setFloat4(string inName, glm::vec4 inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniform4fv(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			1,
			&inValue[0]
		);
	}

	void shader::setMat3(string inName, glm::mat3 inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniformMatrix3fv(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			1,
			GL_FALSE,
			&inValue[0][0]
		);
	}

	void shader::setMat4(string inName, glm::mat4 inValue) const noexcept
	{
		glUseProgram(m_idProgram);
		glUniformMatrix4fv(
			glGetUniformLocation(m_idProgram, inName.c_str()),
			1,
			GL_FALSE,
			&inValue[0][0]
		);
	}
	}
