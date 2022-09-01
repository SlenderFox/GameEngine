#pragma region
#pragma once
#include <fstream>
#include "Light.hpp"
#include "Texture.hpp"
#pragma endregion

namespace Engine
{
	enum class ShaderType : uint8_t
	{
		PROGRAM,
		VERTEX,
		FRAGMENT
	};

	class Shader
	{
	private:
		bool m_shaderLoaded = false;
		unsigned int m_idProgram, m_idVertex, m_idFragment;
		string m_shaderPath;	// The file path of the shaders

		void LoadShader(ShaderType pType);
		bool CompileShader(unsigned int* pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool ShaderErrorChecking(unsigned int* pShaderID, ShaderType pType);
		
	public:
		Shader();
		Shader(string pShaderPath);

		void Destroy(bool pValidate);
		void Use() const;
		void LoadPaths(string pShaderPath);
		bool GetLoaded() const { return m_shaderLoaded; }

		#pragma region Setters
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
		 * @brief Assigns data to a uint uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetUint(const string& pName, unsigned int pValue) const;
		/**
		 * @brief Assigns data to a float uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetFloat(const string& pName, float pValue) const;
		/**
		 * @brief Assigns data to a vec2 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pX The first value assigned to the uniform
		 * @param pY The second value assigned to the uniform
		 */
		void SetVec2(const string& pName, float pX, float pY) const;
		/**
		 * @brief Assigns data to a vec2 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetVec2(const string& pName, glm::vec2 pValue) const;
		/**
		 * @brief Assigns data to a vec2 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform, passed by reference
		 */
		void SetVec2(const string& pName, glm::vec2& pValue) const;
		/**
		 * @brief Assigns data to a vec3 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pX The first value assigned to the uniform
		 * @param pY The second value assigned to the uniform
		 * @param pZ The third value assigned to the uniform
		 */
		void SetVec3(const string& pName, float pX, float pY, float pZ) const;
		/**
		 * @brief Assigns data to a vec3 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetVec3(const string& pName, glm::vec3 pValue) const;
		/**
		 * @brief Assigns data to a vec3 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform, passed by reference
		 */
		void SetVec3(const string& pName, glm::vec3& pValue) const;
		/**
		 * @brief Assigns data to a vec4 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pX The first value assigned to the uniform
		 * @param pY The second value assigned to the uniform
		 * @param pZ The third value assigned to the uniform
		 * @param pW The fourth value assigned to the uniform
		 */
		void SetVec4(const string& pName, float pX, float pY, float pZ, float pW) const;
		/**
		 * @brief Assigns data to a vec4 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetVec4(const string& pName, glm::vec4 pValue) const;
		/**
		 * @brief Set the Vec 4 object
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform, passed by reference
		 */
		void SetVec4(const string& pName, glm::vec4& pValue) const;
		/**
		 * @brief Assigns data to a mat3 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetMat3(const string& pName, glm::mat3 pValue) const;
		/**
		 * @brief Assigns data to a mat3 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform, passed by reference
		 */
		void SetMat3(const string& pName, glm::mat3& pValue) const;
		/**
		 * @brief Assigns data to a mat4 uniform in the shader
		 *
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform
		 */
		void SetMat4(const string& pName, glm::mat4 pValue) const;
		/**
		 * @brief Assigns data to a mat4 uniform in the shader
		 * 
		 * @param pName The name of the uniform
		 * @param pValue The value assigned to the uniform, passed by reference
		 */
		void SetMat4(const string& pName, glm::mat4& pValue) const;
		#pragma endregion
	};
}
