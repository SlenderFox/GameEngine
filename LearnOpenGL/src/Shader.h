#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>

using std::string;
using std::ifstream;

class Shader
{
public:
    // Constructor reads and builds the shader
    Shader(const char* pVertexPath, const char* pFragmentPath);
    ~Shader();
    // Use/activate the shader
    void Use();
    // Utility uniform functions
    void SetBool(const string& pName, bool pValue) const;
    void SetInt(const string& pName, int pValue) const;
    void SetFloat(const string& pName, float pValue) const;
    void SetMat4(const string& pName, glm::mat4 pValue) const;

    unsigned int m_idProgram;

private:
    void ShaderErrorChecking(unsigned int* pShaderID, string pType);

    unsigned int m_idVertex, m_idFragment;
    string m_vertexString, m_fragmentString;
    ifstream m_vertexFile;
    ifstream m_fragmentFile;
};
