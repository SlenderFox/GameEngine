#include "Shader.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
using std::ifstream;

Shader::Shader(const char* pVertexPath, const char* pFragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    string vertexString, fragmentString;
    ifstream vertexFile, fragmentFile;
    // Ensure ifstream objects can throw exceptions:
    vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
    fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        // Open files
        vertexFile.open(pVertexPath);
        fragmentFile.open(pFragmentPath);
        std::stringstream vertexStream, fragmentStream;
        // Read file's buffer contents into streams
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();
        // Close file handlers
        vertexFile.close();
        fragmentFile.close();
        // Convert stream into string
        vertexString = vertexStream.str();
        fragmentString = fragmentStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    // 2. compile shaders
    unsigned int id_Vertex, id_Fragment;

    // Creates the vertex shader object and assigns to an id
    id_Vertex = glCreateShader(GL_VERTEX_SHADER);
    // Loads the vertex shader into the object
    glShaderSource(id_Vertex, 1, &vertexCode, NULL);
    // Compiles the shader at run-time
    glCompileShader(id_Vertex);
    // Performs error checking on the vertex shader
    ShaderErrorChecking(&id_Vertex, "VERTEX");

    // Creates the fragment shader object and assigns to an id
    id_Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    // Loads the fragment shader into the object
    glShaderSource(id_Fragment, 1, &fragmentCode, NULL);
    // Compiles the fragment at run-time
    glCompileShader(id_Fragment);
    // Performs error checking on the fragment shader
    ShaderErrorChecking(&id_Fragment, "FRAGMENT");

    // Creates a shader program object
    m_ID = glCreateProgram();
    // Link the vertex and fragment shaders
    glAttachShader(m_ID, id_Vertex);
    glAttachShader(m_ID, id_Fragment);
    glLinkProgram(m_ID);
    // Performs error checking on the shader program
    ShaderErrorChecking(&m_ID, "PROGRAM");
    // We no longer need the vertex and fragment shaders
    glDeleteShader(id_Vertex);
    glDeleteShader(id_Fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::use()
{
    glUseProgram(m_ID);
}

void Shader::setBool(const string& pName, bool pValue) const
{
    glUniform1i(glGetUniformLocation(m_ID, pName.c_str()), (int)pValue);
}

void Shader::setInt(const string& pName, int pValue) const
{
    glUniform1i(glGetUniformLocation(m_ID, pName.c_str()), pValue);
}

void Shader::setFloat(const string& pName, float pValue) const
{
    glUniform1f(glGetUniformLocation(m_ID, pName.c_str()), pValue);
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
