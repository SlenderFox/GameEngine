#pragma once
#include <string>
#include <fstream>

using std::string;
using std::ifstream;

class Shader
{
public:
    // Constructor reads and builds the shader
    Shader(const char* pVertexPath, const char* pFragmentPath);
    ~Shader();
    // Use/activate the shader
    void use();
    // Utility uniform functions
    void setBool(const string& pName, bool pValue) const;
    void setInt(const string& pName, int pValue) const;
    void setFloat(const string& pName, float pValue) const;

    unsigned int m_idProgram;

private:
    void ShaderErrorChecking(unsigned int* pShaderID, string pType);

    unsigned int m_idVertex, m_idFragment;
    string m_vertexString, m_fragmentString;
    ifstream m_vertexFile;
    ifstream m_fragmentFile;
};
