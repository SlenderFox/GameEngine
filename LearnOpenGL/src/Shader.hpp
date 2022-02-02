#pragma region
#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>

using std::string;
#pragma endregion

namespace Engine
{
	enum class ShaderType : unsigned short
	{
		PROGRAM,
		VERTEX,
		FRAGMENT
	};

	class Shader
	{
	public:
		Shader();
		/**
		 * @brief Construct a new Shader object
		 *
		 * @param pVertexPath The file path to the vertex shader
		 * @param pFragmentPath The file path to the fragment shader
		 */
		Shader(string pVertexPath, string pFragmentPath);

		// Copy constructors
		Shader(const Shader& pOther);
		Shader(Shader&& pOther);
		Shader& operator=(const Shader& pOther);
		Shader& operator=(Shader&& pOther);

		~Shader() {}

		/**
		 * @brief Destroys the shader but only if it was ever initliased
		 *
		 * @param pValidate Whether the shader was even initialised
		 */
		void Destroy(bool pValidate);
		/**
		 * @brief Use/activate the shader
		 */
		void Use();
		void LoadPaths(string pVertexPath, string pFragmentPath);

		// Utility uniform functions
		/**
		 * @brief Assigns data to a bool uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetBool(const string& pName, bool pValue) const;
		/**
		 * @brief Assigns data to a int uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetInt(const string& pName, int pValue) const;
		/**
		 * @brief Assigns data to a float uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetFloat(const string& pName, float pValue) const;
		/**
		 * @brief Assigns data to a mat4 uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetMat4(const string& pName, glm::mat4 pValue) const;

		bool GetLoaded() const { return m_shaderLoaded; }

	private:
		void LoadShader(ShaderType pType);
		bool CompileShader(unsigned int* pId, ShaderType pType, const char* pCode);
		//void LoadVertexShader();
		//void LoadFragmentShader();
		//bool CompileVertexShader(const char* pVertexCode);
		//bool CompileFragmentShader(const char* pFragmentCode);
		void CreateShaderProgram();

		/**
		 * @brief Checks a shader for errors while loading and logs them
		 *
		 * @param pShaderID The id of the shader to be error checked
		 * @param pType The type of shader being error checked
		 * @return true if no error, false if error
		 */
		bool ShaderErrorChecking(unsigned int* pShaderID, ShaderType pType);

		bool m_shaderLoaded = false;
		unsigned int m_idProgram, m_idVertex, m_idFragment;
		string m_vertexPath, m_fragmentPath;	// The file path of the shaders
	};
}