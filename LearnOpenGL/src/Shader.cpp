#include "Shader.h"
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;

namespace Engine
{
    Shader::Shader(const char* pVertexPath, const char* pFragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        // Ensure ifstream objects can throw exceptions:
        m_vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
        m_fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);
        try
        {
            /* For both vertex and frament we:
            * Open files
            * Read file's buffer contents into streams
            * Close file handlers
            * Convert stream into string
            */
            std::stringstream vertexStream, fragmentStream;
            // Vertex
            m_vertexFile.open(pVertexPath);
            vertexStream << m_vertexFile.rdbuf();
            m_vertexFile.close();
            m_vertexString = vertexStream.str();
            // Fragment
            m_fragmentFile.open(pFragmentPath);
            fragmentStream << m_fragmentFile.rdbuf();
            m_fragmentFile.close();
            m_fragmentString = fragmentStream.str();
        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }
        const char* vertexCode = m_vertexString.c_str();
        const char* fragmentCode = m_fragmentString.c_str();

        // 2. compile shaders
        // Creates the vertex shader object and assigns to an id
        m_idVertex = glCreateShader(GL_VERTEX_SHADER);
        // Loads the vertex shader into the object
        glShaderSource(m_idVertex, 1, &vertexCode, NULL);
        // Compiles the shader at run-time
        glCompileShader(m_idVertex);
        // Performs error checking on the vertex shader
        ShaderErrorChecking(&m_idVertex, "VERTEX");

        // Creates the fragment shader object and assigns to an id
        m_idFragment = glCreateShader(GL_FRAGMENT_SHADER);
        // Loads the fragment shader into the object
        glShaderSource(m_idFragment, 1, &fragmentCode, NULL);
        // Compiles the fragment at run-time
        glCompileShader(m_idFragment);
        // Performs error checking on the fragment shader
        ShaderErrorChecking(&m_idFragment, "FRAGMENT");

        // Creates a shader program object
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
                cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
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
                cout << "ERROR::SHADER::" << pType << "::COMPILATION_FAILED\n" << infoLog << endl;
            }
        }
    }
}
