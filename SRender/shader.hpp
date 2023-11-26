#pragma once
#include <string>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** A shader is used to render the given vertex and texture information to the screen
 * @todo Allow a new shader to be loaded in the same object
 * @todo Make loadShader() clearer to read
 * @todo Remove glm include and have shader sets take an inbuilt type
*/
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

	inline void loadShader(const shaderType _type);

	_NODISCARD inline bool compileShader(
		uint32_t *_id,
		shaderType _type,
		const char *_code
	) noexcept;

	inline void createShaderProgram() noexcept;

	_NODISCARD inline bool checkForErrors(
		const uint32_t _shaderID,
		const shaderType _type
	) const noexcept;

	/** Select between two outputs based on the shadertype input
	 * @tparam T Arbitrary type allows this function to select for many different types
	 * @param _type The input shader type, must be either vertex or fragment
	 * @param _vertex The output if the type is vertex
	 * @param _fragment The output if the type is fragment
	 * @return [T] One of the two given params
	 * @note Will assert that _type is not shaderType::program
	 */
	template<typename T>
	_NODISCARD inline T byType(
		const shaderType _type,
		T _vertex,
		T _fragment
	) const noexcept;

public:
	shader(const std::string *_shaderPath = nullptr);
	~shader();

	/** Deletes the currently used shader program */
	void destroy() noexcept;
	/** Given a path, will load and compile a shader
	 * @param _shaderPath The relative path of the vertex and fragment shader and their name
	 * @note The vertex and fragment shaders must have the same name,
	 * the path does no require the extension
	 */
	void load(const std::string *_shaderPath = nullptr);
	/** Makes this shader the active shader */
	void use() const noexcept;

	_NODISCARD constexpr bool isLoaded() const noexcept;

	void setBool   (const std::string _name, const bool       _value) const noexcept;
	void setInt    (const std::string _name, const int32_t    _value) const noexcept;
	void setUint   (const std::string _name, const uint32_t   _value) const noexcept;
	void setFloat  (const std::string _name, const float      _value) const noexcept;
	void setFloat2 (const std::string _name, const glm::vec2 *_value) const noexcept;
	void setFloat2 (const std::string _name, const glm::vec2  _value) const noexcept;
	void setFloat3 (const std::string _name, const glm::vec3 *_value) const noexcept;
	void setFloat3 (const std::string _name, const glm::vec3  _value) const noexcept;
	void setFloat4 (const std::string _name, const glm::vec4 *_value) const noexcept;
	void setFloat4 (const std::string _name, const glm::vec4  _value) const noexcept;
	void setMat3   (const std::string _name, const glm::mat3 *_value) const noexcept;
	void setMat3   (const std::string _name, const glm::mat3  _value) const noexcept;
	void setMat4   (const std::string _name, const glm::mat4 *_value) const noexcept;
	void setMat4   (const std::string _name, const glm::mat4  _value) const noexcept;
};
}
