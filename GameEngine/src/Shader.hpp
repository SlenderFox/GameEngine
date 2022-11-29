#pragma once
#include "Camera.hpp"
#include "Texture.hpp"

namespace Engine
{
	/** A shader is used to render the given vertex and texture information to the screen */
	struct Shader
	{
	private:
		/** Used for selecting which part of the shader to use for functions */
		enum class ShaderType: uint8_t
		{
			Program,
			Vertex,
			Fragment
		};

		bool m_shaderLoaded = false;
		uint32_t m_idProgram, m_idVertex, m_idFragment;
		std::string m_shaderPath;	// The file inath of the shaders

		inline void LoadShader(const ShaderType inType);

		inline bool CompileShader(
			uint32_t* inId,
			ShaderType inType,
			const char* inCode
		) noexcept;

		inline void CreateShaderProgram() noexcept;

		_NODISCARD inline
		bool CheckForErrors(
			const uint32_t* inShaderID,
			const ShaderType inType
		) const noexcept;

		/** Select between two outputs based on the shadertype input
		 * @tparam T Arbitrary type allows this function to select for many different types
		 * @param inType The input shader type, must be either Vertex or Fragment
		 * @param inVertex The output if the type is Vertex
		 * @param inFragment The output if the type is Fragment
		 * @return [T] One of the two given params
		 * @note Will assert that inType is not ShaderType::Program
		 */
		template<typename T> _NODISCARD inline
		T ByType(
			const ShaderType inType,
			T inVertex,
			T inFragment
		) const noexcept;

	public:
		Shader(const std::string* inShaderPath = nullptr);
		~Shader();

		/** Deletes the currently used shader program */
		void Destroy() noexcept;
		/** Given a path, will load and compile a shader
		 * @param inShaderPath The relative path of the vertex and fragment shader and their name
		 * @note The vertex and fragment shaders must have the same name,
		 * the path does no require the extension
		 */
		void Load(const std::string* inShaderPath = nullptr);
		/** Makes this shader the active shader */
		void Use() const noexcept;

		_NODISCARD constexpr
		bool IsLoaded() const noexcept;

		#pragma region Setters
		void SetBool  (const std::string inName, const bool      inValue) const noexcept;
		void SetInt   (const std::string inName, const int32_t   inValue) const noexcept;
		void SetUint  (const std::string inName, const uint32_t  inValue) const noexcept;
		void SetFloat (const std::string inName, const float     inValue) const noexcept;
		void SetVec2  (const std::string inName, const glm::vec2 inValue) const noexcept;
		void SetVec3  (const std::string inName, const glm::vec3 inValue) const noexcept;
		void SetVec4  (const std::string inName, const glm::vec4 inValue) const noexcept;
		void SetMat3  (const std::string inName, const glm::mat3 inValue) const noexcept;
		void SetMat4  (const std::string inName, const glm::mat4 inValue) const noexcept;
		#pragma endregion
	};
}
