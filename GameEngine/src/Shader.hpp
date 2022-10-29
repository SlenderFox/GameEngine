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

		void Load(std::string pShaderPath);
		void LoadShader(ShaderType pType);
		bool CompileShader(uint32_t* pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool CheckForErrors(const uint32_t* pShaderID, ShaderType pType) noexcept;
		template<typename T>
		constexpr T GetType(ShaderType pType, T ifVertex, T ifFragment) const;

	public:
		Shader(std::string pShaderPath = "");
		~Shader();

		void Use() const noexcept;
		bool IsLoaded() const noexcept { return m_shaderLoaded; }

#		pragma region Setters
		// Utility uniform functions
		void SetBool	(std::string pName, bool		pValue) const noexcept;
		void SetInt		(std::string pName, int32_t	pValue) const noexcept;
		void SetUint	(std::string pName, uint32_t	pValue) const noexcept;
		void SetFloat	(std::string pName, float		pValue) const noexcept;
		void SetVec2	(std::string pName, glm::vec2 pValue) const noexcept;
		void SetVec3	(std::string pName, glm::vec3 pValue) const noexcept;
		void SetVec4	(std::string pName, glm::vec4 pValue) const noexcept;
		void SetMat3	(std::string pName, glm::mat3 pValue) const noexcept;
		void SetMat4	(std::string pName, glm::mat4 pValue) const noexcept;
#		pragma endregion
	};
}
