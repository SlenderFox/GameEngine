/** Designed to be the one stop shop for swapping graphics libraries */
#include <vector>
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
		void clear() noexcept;
		void deleteMesh(
			const uint32_t _idVAO,
			const uint32_t _idVBO,
			const uint32_t _idEBO
		) noexcept;
		void deleteShader(const uint32_t _idProgram) noexcept;
		void deleteTextures(const uint32_t _textureCount, const uint32_t *_textureIds) noexcept;
		void drawElements(const uint32_t _idVAO, const uint32_t _size) noexcept;
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
		void setClearColour(
			const float _r,
			const float _g,
			const float _b,
			const float _a = 1.0f
		) noexcept;
		void setRenderMode(const int _mode) noexcept;
		void setResolution(const size_t _width, const size_t _height) noexcept;
	}
}
