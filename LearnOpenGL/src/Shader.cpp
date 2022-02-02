#include "Shader.hpp"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

namespace Engine
{
	Shader::Shader()
	{

	}

	Shader::Shader(string pVertexPath, string pFragmentPath)
	{
		LoadPaths(pVertexPath, pFragmentPath);
	}

	Shader::Shader(const Shader& pOther)
	{
		m_vertexPath = pOther.m_vertexPath;
		m_fragmentPath = pOther.m_fragmentPath;
	}
	
	Shader::Shader(Shader&& pOther)
	{
		m_vertexPath = pOther.m_vertexPath;
		m_fragmentPath = pOther.m_fragmentPath;
	}
	
	Shader& Shader::operator=(const Shader& pOther)
	{
		Shader* newObj = new Shader(pOther.m_vertexPath, pOther.m_fragmentPath);
		return *newObj;
	}
	
	Shader& Shader::operator=(Shader&& pOther)
	{
		Shader* newObj = new Shader(pOther.m_vertexPath, pOther.m_fragmentPath);
		return *newObj;
	}

	void Shader::Destroy(bool pValidate)
	{
		if (pValidate && m_shaderLoaded)
			glDeleteProgram(m_idProgram);
	}

	void Shader::LoadPaths(string pVertexPath, string pFragmentPath)
	{
		m_vertexPath = pVertexPath;
		m_fragmentPath = pFragmentPath;

		CreateVertexShader();
		CreatFragmentShader();
		CreateShaderProgram();
	}

	void Shader::Use()
	{
		glUseProgram(m_idProgram);
	}

	void Shader::CreateVertexShader()
	{
		// Must be defined out here
		const char* vertexCode;
		ifstream m_vertexFile;
		string m_vertexString;

		// Ensure ifstream objects can throw exceptions
		m_vertexFile.exceptions(ifstream::failbit | ifstream::badbit);

		// 1. retrieve the vertex source code from filePath
		try
		{
			/* For both vertex and fragment we:
			* Open files
			* Read file's buffer contents into streams
			* Close file handlers
			* Convert stream into string
			*/
			std::stringstream vertexStream;
			m_vertexFile.open(m_vertexPath);
			vertexStream << m_vertexFile.rdbuf();
			m_vertexFile.close();
			m_vertexString = vertexStream.str();
			vertexCode = m_vertexString.c_str();
		}
		catch (ifstream::failure e)
		{
#ifdef _DEBUG
			cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_READ" << endl;
#endif

			// Fallback code
			vertexCode = "#version 330 core\n\
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
		}

		// 2. compile shaders
		// Creates the vertex shader object and assigns to an id
		m_idVertex = glCreateShader(GL_VERTEX_SHADER);
		// Loads the vertex shader into the object
		glShaderSource(m_idVertex, 1, &vertexCode, NULL);
		// Compiles the shader at run-time
		glCompileShader(m_idVertex);
		// Performs error checking on the vertex shader
		ShaderErrorChecking(&m_idVertex, "VERTEX");
	}

	void Shader::CreatFragmentShader()
	{
		// Must be defined out here
		const char* fragmentCode;
		ifstream m_fragmentFile;
		string m_fragmentString;

		// Ensure ifstream objects can throw exceptions
		m_fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);

		// 1. retrieve the fragment source code from filePath
		try
		{
			/* For both vertex and fragment we:
			* Open files
			* Read file's buffer contents into streams
			* Close file handlers
			* Convert stream into string
			*/
			std::stringstream fragmentStream;
			m_fragmentFile.open(m_fragmentPath);
			fragmentStream << m_fragmentFile.rdbuf();
			m_fragmentFile.close();
			m_fragmentString = fragmentStream.str();
			fragmentCode = m_fragmentString.c_str();
		}
		catch (ifstream::failure e)
		{
#ifdef _DEBUG
			cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_READ" << endl;
#endif

			// Fallback code
			fragmentCode = "#version 330 core\n\
			out vec4 FragCol;\
			in vec3 colour;\
			in vec2 texCoord;\
			void main() {\
			 FragCol = vec4(colour, 1);\
			}";
		}

		// 2. compile shaders
		// Creates the fragment shader object and assigns to an id
		m_idFragment = glCreateShader(GL_FRAGMENT_SHADER);
		// Loads the fragment shader into the object
		glShaderSource(m_idFragment, 1, &fragmentCode, NULL);
		// Compiles the fragment at run-time
		glCompileShader(m_idFragment);
		// Performs error checking on the fragment shader
		ShaderErrorChecking(&m_idFragment, "FRAGMENT");
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
		ShaderErrorChecking(&m_idProgram, "PROGRAM");
		// We no longer need the vertex and fragment shaders
		glDeleteShader(m_idVertex);
		glDeleteShader(m_idFragment);

		// Sets the shader as the active one
		glUseProgram(m_idProgram);

		m_shaderLoaded = true;
	}
		
	void Shader::ShaderErrorChecking(unsigned int* pShaderID, string pType)
	{
		// Variables used in error checking and handling
		int success;
		char infoLog[512];

		if (pType == "PROGRAM")
		{
			// Retrieves the compile status of the given shader by id
			glGetProgramiv(*pShaderID, GL_LINK_STATUS, &success);
			if (!success)
			{
				// In the case of a failure it loads the log and outputs
				glGetProgramInfoLog(*pShaderID, 512, NULL, infoLog);
#ifdef _DEBUG
				cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
#endif
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
				cout << "ERROR::SHADER::" << pType << "::COMPILATION_FAILED\n" << infoLog << endl;
#endif
			}
		}
	}

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

	void Shader::SetMat4(const string& pName, glm::mat4 pValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_idProgram, pName.c_str()), 1, GL_FALSE, glm::value_ptr(pValue));
	}
}
