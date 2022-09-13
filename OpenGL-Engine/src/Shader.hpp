#pragma region
#pragma once
#include "Light.hpp"
#include "Texture.hpp"
#pragma endregion

namespace Engine
{
	enum class ShaderType: uint8_t
	{
		PROGRAM,
		VERTEX,
		FRAGMENT
	};

	class Shader
	{
	private:
		bool m_shaderLoaded = false;
		uint32_t m_idProgram, m_idVertex, m_idFragment;
		std::string m_shaderPath;	// The file path of the shaders

		void LoadShader(ShaderType pType);
		bool CompileShader(uint32_t* pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool ShaderErrorChecking(uint32_t* pShaderID, ShaderType pType);
		
	public:
		Shader();
		Shader(std::string pShaderPath);

		void Destroy(bool pValidate);
		void Use() const;
		void LoadPaths(std::string pShaderPath);
		bool GetLoaded() const { return m_shaderLoaded; }

		#pragma region Setters
		// Utility uniform functions
		void SetBool	(const std::string& pName, const bool&			pValue) const;
		void SetInt		(const std::string& pName, const int32_t&		pValue) const;
		void SetUint	(const std::string& pName, const uint32_t&		pValue) const;
		void SetFloat	(const std::string& pName, const float&			pValue) const;
		void SetVec2	(const std::string& pName, const glm::vec2&		pValue) const;
		void SetVec3	(const std::string& pName, const glm::vec3&		pValue) const;
		void SetVec4	(const std::string& pName, const glm::vec4&		pValue) const;
		void SetMat3	(const std::string& pName, const glm::mat3&		pValue) const;
		void SetMat4	(const std::string& pName, const glm::mat4&		pValue) const;
		#pragma endregion
	};
}
