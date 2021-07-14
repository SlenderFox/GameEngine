#pragma once
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::ifstream;

class Shader
{
public:
    // Constructor reads and builds the shader
    Shader(const char* pVertexPath, const char* pFragmentPath);
    ~Shader() { glDeleteProgram(m_idProgram); }
    // Use/activate the shader
    void Use() { glUseProgram(m_idProgram); }
    // Utility uniform functions
    void SetBool(const string& pName, bool pValue) const;
    void SetInt(const string& pName, int pValue) const;
    void SetFloat(const string& pName, float pValue) const;
    void SetMat4(const string& pName, glm::mat4 pValue) const;

    //unsigned int m_idProgram;

private:
    void ShaderErrorChecking(unsigned int* pShaderID, string pType);

    unsigned int m_idProgram, m_idVertex, m_idFragment;
    string m_vertexString, m_fragmentString;
    ifstream m_vertexFile;
    ifstream m_fragmentFile;
};
