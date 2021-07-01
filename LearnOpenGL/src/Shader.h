#pragma once
#include <string>

using std::string;

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

    // The program ID
    unsigned int m_ID;

private:
    void ShaderErrorChecking(unsigned int* pShaderID, string pType);
};
