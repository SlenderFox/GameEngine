#pragma once
#include "Camera.hpp"
#include "Texture.hpp"

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
		bool m_shaderLoaded = false, m_usingFallback = false;
		uint32_t m_idProgram, m_idVertex, m_idFragment;
		std::string m_shaderPath;	// The file path of the shaders

		void Load(const std::string& pShaderPath);
		void LoadShader(ShaderType pType);
		bool CompileShader(uint32_t* const& pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool CheckForErrors(const uint32_t* const& pShaderID, ShaderType pType) noexcept;
		template<typename T>
		constexpr T GetType(ShaderType pType, T ifVertex, T ifFragment) const;
		
	public:
		Shader(const std::string& pShaderPath = "");
		~Shader();

		void Use() const noexcept;
		bool IsLoaded() const noexcept { return m_shaderLoaded; }

		#pragma region Setters
		// Utility uniform functions
		void SetBool	(const std::string& pName, const bool&		pValue) const noexcept;
		void SetInt		(const std::string& pName, const int32_t&	pValue) const noexcept;
		void SetUint	(const std::string& pName, const uint32_t&	pValue) const noexcept;
		void SetFloat	(const std::string& pName, const float&		pValue) const noexcept;
		void SetVec2	(const std::string& pName, const glm::vec2&	pValue) const noexcept;
		void SetVec3	(const std::string& pName, const glm::vec3&	pValue) const noexcept;
		void SetVec4	(const std::string& pName, const glm::vec4&	pValue) const noexcept;
		void SetMat3	(const std::string& pName, const glm::mat3&	pValue) const noexcept;
		void SetMat4	(const std::string& pName, const glm::mat4&	pValue) const noexcept;
		#pragma endregion
	};
}
