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
using glm::vec3;
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

	Shader::Shader(string pVertexPath, string pFragmentPath)
	{
		LoadPaths(pVertexPath, pFragmentPath);
	}

	#pragma region Copy constructors
	Shader::Shader(const Shader& pOther)
	{
		m_vertexPath = pOther.m_vertexPath;
		m_fragmentPath = pOther.m_fragmentPath;
	}
	
	Shader::Shader(Shader&& pOther) noexcept
	{
		m_vertexPath = pOther.m_vertexPath;
		m_fragmentPath = pOther.m_fragmentPath;
	}
	
	Shader& Shader::operator=(const Shader& pOther)
	{
		Shader* newObj = new Shader(pOther.m_vertexPath, pOther.m_fragmentPath);
		return *newObj;
	}
	
	Shader& Shader::operator=(Shader&& pOther) noexcept
	{
		Shader* newObj = new Shader(pOther.m_vertexPath, pOther.m_fragmentPath);
		return *newObj;
	}
	#pragma endregion

	void Shader::Destroy(bool pValidate)
	{
		if (pValidate && m_shaderLoaded)
			glDeleteProgram(m_idProgram);
	}

	void Shader::Use()
	{
		glUseProgram(m_idProgram);
	}

	void Shader::LoadPaths(string pVertexPath, string pFragmentPath)
	{
		m_vertexPath = pVertexPath;
		m_fragmentPath = pFragmentPath;

		//LoadVertexShader();
		//LoadFragmentShader();
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
		 layout (location = 0) in vec3 aPos;\
		 layout (location = 1) in vec3 aCol;\
		 layout (location = 2) in vec2 aTexCoord;\
		 out vec3 colour;\
		 out vec2 texCoord;\
		 uniform mat4 camera;\
		 uniform mat4 model;\
		 void main() {\
		  gl_Position = camera * model * vec4(aPos, 1.0);\
		  colour = aCol;\
		  texCoord = aTexCoord;\
		 }";
		 const char* fragmentFallback = "#version 330 core\n\
		 out vec4 FragCol;\
		 in vec3 colour;\
		 in vec2 texCoord;\
		 void main() {\
		  FragCol = vec4(colour, 1);\
		 }";
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
			inStream.open((pType == ShaderType::VERTEX ? m_vertexPath : m_fragmentPath));
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
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), (int)pValue);
	}

	void Shader::SetInt(const string& pName, int pValue) const
	{
		glUniform1i(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetFloat(const string& pName, float pValue) const
	{
		glUniform1f(glGetUniformLocation(m_idProgram, pName.c_str()), pValue);
	}

	void Shader::SetVec3(const string& pName, vec3 pValue) const
	{
		glUniform3f(glGetUniformLocation(m_idProgram, pName.c_str()), pValue.x, pValue.y, pValue.z);
	}

	void Shader::SetMat3(const string& pName, glm::mat3 pValue) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, glm::value_ptr(pValue));
	}

	void Shader::SetMat4(const string& pName, glm::mat4 pValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, glm::value_ptr(pValue));
	}
	#pragma endregion
}
