#pragma region
#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>

using std::string;
using std::ifstream;
#pragma endregion

namespace Engine
{
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
		void LoadPaths(string pVertexPath, string pFragmentPath);
		/**
		 * @brief Use/activate the shader
		 */
		void Use();

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
		void CreateVertexShader();
		void CreatFragmentShader();
		void CreateShaderProgram();

		/**
		 * @brief Checks a shader for errors while loading and logs them
		 *
		 * @param pShaderID The id of the shader to be error checked
		 * @param pType The type of shader being error checked
		 */
		void ShaderErrorChecking(unsigned int* pShaderID, string pType);

		bool m_shaderLoaded = false;
		unsigned int m_idProgram, m_idVertex, m_idFragment;
		string m_vertexPath, m_fragmentPath;	// The file path of the shaders
	};
}
