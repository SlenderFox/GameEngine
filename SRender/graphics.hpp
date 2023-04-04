/** Designed to be the one stop shop for swapping graphics libraries */
#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	namespace graphics
	{
		_NODISCARD const bool loadGlad() noexcept;
		_NODISCARD const bool getGladLoaded() noexcept;
		void clearScreenBuffers() noexcept;
		void setClearColour(
			const float _r,
			const float _g,
			const float _b,
			const float _a = 1.0f
		) noexcept;
		void setRenderMode(const int _mode) noexcept;
		void setResolution(const size_t _width, const size_t _height) noexcept;

		// Mesh

		void setupMesh(
			uint32_t *_outIdVAO,
			uint32_t *_outIdVBO,
			uint32_t *_outIdEBO,
			const float *_vertices,
			const uint32_t *_indices,
			const uint32_t _verticesByteSize,
			const uint32_t _indicesByteSize,
			const uint32_t _vertexSize,
			uint64_t _normalOffset,
			uint64_t _texCoordOffset
		) noexcept;
		void deleteMesh(
			const uint32_t _idVAO,
			const uint32_t _idVBO,
			const uint32_t _idEBO
		) noexcept;
		void drawElements(const uint32_t _idVAO, const uint32_t _size) noexcept;

		// Texture

		void deleteTextures(const uint32_t _textureCount, const uint32_t *_textureIds) noexcept;

		// Shader

		void deleteShaderProgram(const uint32_t _idProgram) noexcept;
		void deleteShader(const uint32_t _idShader) noexcept;
		uint32_t createVertexShader() noexcept;
		uint32_t createFragmentShader() noexcept;
		uint32_t createShaderProgram(uint32_t _idVertex, uint32_t _idFragment) noexcept;
		void useShaderProgram(uint32_t _idProgram) noexcept;
		void loadShaderSource(uint32_t _idShader, const char *_code) noexcept;
		void compileShader(uint32_t _idShader) noexcept;
		void getProgramiv(uint32_t _idShader, int32_t *_success) noexcept;
		void getProgramInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept;
		void getShaderiv(uint32_t _idShader, int32_t *_success) noexcept;
		void getShaderInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept;

		int32_t getUniformLocation(uint32_t _idProgram, std::string _name) noexcept;

		void setBool(uint32_t _idProgram, int32_t _location, bool _value) noexcept;
		void setInt(uint32_t _idProgram, int32_t _location, int32_t _value) noexcept;
		void setUint(uint32_t _idProgram, int32_t _location, uint32_t _value) noexcept;
		void setFloat(uint32_t _idProgram, int32_t _location, float _value) noexcept;
		void setFloat2(uint32_t _idProgram, int32_t _location, float *_value) noexcept;
		void setFloat3(uint32_t _idProgram, int32_t _location, float *_value) noexcept;
		void setFloat4(uint32_t _idProgram, int32_t _location, float *_value) noexcept;
		void setMat3(uint32_t _idProgram, int32_t _location, float *_value) noexcept;
		void setMat4(uint32_t _idProgram, int32_t _location, float *_value) noexcept;
	}
}
