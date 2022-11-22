#pragma once
#include "Camera.hpp"
#include "Texture.hpp"

namespace Engine
{
	class Shader
	{
	private:
		/** Used for selecting which shader to use for functions */
		enum class ShaderType: uint8_t
		{
			Program,
			Vertex,
			Fragment
		};

		bool m_shaderLoaded = false;
		uint32_t m_idProgram, m_idVertex, m_idFragment;
		std::string m_shaderPath;	// The file path of the shaders

		void Load(std::string pShaderPath);
		void LoadShader(const ShaderType pType);
		bool CompileShader(uint32_t* pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool CheckForErrors(const uint32_t* pShaderID, const ShaderType pType) noexcept;
		/** Select between two outputs based on the shadertype input
		 * @tparam T Arbitrary type allows this function to select for many different types
		 * @param pType The input shader type, must be either Vertex or Fragment
		 * @param ifVertex The output if the type is Vertex
		 * @param ifFragment The output if the type is Fragment
		 * @return [T] One of the two given params
		 * @exception Will fail if given ShaderType::Program
		 */
		template<typename T>
		T GetType(const ShaderType pType, T ifVertex, T ifFragment) const
		{
			assert(pType != ShaderType::Program && "Incorrect shadertype passed");
			return (pType == ShaderType::Vertex ? ifVertex : ifFragment);
		}

	public:
		Shader(const std::string* pShaderPath = nullptr);
		~Shader();

		void Use() const noexcept;
		bool IsLoaded() const noexcept { return m_shaderLoaded; }

		#pragma region Setters
		void SetBool	(const std::string pName, const bool		pValue) const noexcept;
		void SetInt		(const std::string pName, const int32_t	pValue) const noexcept;
		void SetUint	(const std::string pName, const uint32_t	pValue) const noexcept;
		void SetFloat	(const std::string pName, const float		pValue) const noexcept;
		void SetVec2	(const std::string pName, const glm::vec2 pValue) const noexcept;
		void SetVec3	(const std::string pName, const glm::vec3 pValue) const noexcept;
		void SetVec4	(const std::string pName, const glm::vec4 pValue) const noexcept;
		void SetMat3	(const std::string pName, const glm::mat3 pValue) const noexcept;
		void SetMat4	(const std::string pName, const glm::mat4 pValue) const noexcept;
		#pragma endregion
	};
}
