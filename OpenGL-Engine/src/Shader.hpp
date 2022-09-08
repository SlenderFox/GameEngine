#pragma region
#pragma once
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
		std::string m_shaderPath;	// The file path of the shaders

		void LoadShader(ShaderType pType);
		bool CompileShader(unsigned int* pId, ShaderType pType, const char* pCode);
		void CreateShaderProgram();
		bool ShaderErrorChecking(unsigned int* pShaderID, ShaderType pType);
		
	public:
		Shader();
		Shader(std::string pShaderPath);

		void Destroy(bool pValidate);
		void Use() const;
		void LoadPaths(std::string pShaderPath);
		bool GetLoaded() const { return m_shaderLoaded; }

		#pragma region Setters
		// Utility uniform functions
		void SetBool(const std::string& pName, bool pValue) const;
		void SetInt(const std::string& pName, int pValue) const;
		void SetUint(const std::string& pName, unsigned int pValue) const;
		void SetFloat(const std::string& pName, float pValue) const;
		void SetVec2(const std::string& pName, float pX, float pY) const;
		void SetVec2(const std::string& pName, glm::vec2 pValue) const;
		void SetVec2(const std::string& pName, glm::vec2& pValue) const;
		void SetVec3(const std::string& pName, float pX, float pY, float pZ) const;
		void SetVec3(const std::string& pName, glm::vec3 pValue) const;
		void SetVec3(const std::string& pName, glm::vec3& pValue) const;
		void SetVec4(const std::string& pName, float pX, float pY, float pZ, float pW) const;
		void SetVec4(const std::string& pName, glm::vec4 pValue) const;
		void SetVec4(const std::string& pName, glm::vec4& pValue) const;
		void SetMat3(const std::string& pName, glm::mat3 pValue) const;
		void SetMat3(const std::string& pName, glm::mat3& pValue) const;
		void SetMat4(const std::string& pName, glm::mat4 pValue) const;
		void SetMat4(const std::string& pName, glm::mat4& pValue) const;
		#pragma endregion
	};
}
