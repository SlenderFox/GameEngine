#pragma once
#include "camera.hpp"
#include "texture.hpp"

namespace srender
{
	/** A shader is used to render the given vertex and texture information to the screen */
	struct shader
	{
	private:
		/** Used for selecting which part of the shader to use for functions */
		enum class shaderType: uint8_t
		{
			program,
			vertex,
			fragment
		};

		bool m_shaderLoaded = false;
		uint32_t m_idProgram, m_idVertex, m_idFragment;
		std::string m_shaderPath;	// The file inath of the shaders

		inline void loadShader(const shaderType inType);

		inline bool compileShader(
			uint32_t *inId,
			shaderType inType,
			const char *inCode
		) noexcept;

		inline void createShaderProgram() noexcept;

		[[nodiscard]] inline
		bool checkForErrors(
			const uint32_t *inShaderID,
			const shaderType inType
		) const noexcept;

		/** Select between two outputs based on the shadertype input
		 * @tparam T Arbitrary type allows this function to select for many different types
		 * @param inType The input shader type, must be either vertex or fragment
		 * @param inVertex The output if the type is vertex
		 * @param inFragment The output if the type is fragment
		 * @return [T] One of the two given params
		 * @note Will assert that inType is not shaderType::program
		 */
		template<typename T> [[nodiscard]] inline
		T byType(
			const shaderType inType,
			T inVertex,
			T inFragment
		) const noexcept;

	public:
		shader(const std::string *inShaderPath = nullptr);
		~shader();

		/** Deletes the currently used shader program */
		void destroy() noexcept;
		/** Given a path, will load and compile a shader
		 * @param inShaderPath The relative path of the vertex and fragment shader and their name
		 * @note The vertex and fragment shaders must have the same name,
		 * the path does no require the extension
		 */
		void load(const std::string *inShaderPath = nullptr);
		/** Makes this shader the active shader */
		void use() const noexcept;

		[[nodiscard]] constexpr
		bool isLoaded() const noexcept;

		void setBool   (const std::string inName, const bool      inValue) const noexcept;
		void setInt    (const std::string inName, const int32_t   inValue) const noexcept;
		void setUint   (const std::string inName, const uint32_t  inValue) const noexcept;
		void setFloat  (const std::string inName, const float     inValue) const noexcept;
		void setFloat2 (const std::string inName, const glm::vec2 inValue) const noexcept;
		void setFloat3 (const std::string inName, const glm::vec3 inValue) const noexcept;
		void setFloat4 (const std::string inName, const glm::vec4 inValue) const noexcept;
		void setMat3   (const std::string inName, const glm::mat3 inValue) const noexcept;
		void setMat4   (const std::string inName, const glm::mat4 inValue) const noexcept;
	};
}
