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
		_NODISCARD const bool gladLoaded() noexcept;
		void clear() noexcept;
		void deleteMesh(uint32_t _idVAO, uint32_t _idVBO, uint32_t _idEBO) noexcept;
		void deleteShader(uint32_t _idProgram) noexcept;
		void deleteTextures(uint32_t _textureCount, uint32_t *_textureIds) noexcept;
		void drawElements(uint32_t _idVAO, std::vector<uint32_t> *_indices) noexcept;
		void setupMesh(
			uint32_t *_idVAO,
			uint32_t *_idVBO,
			uint32_t *_idEBO,
			float *_vertices,
			uint32_t *_indices,
			int _verticesByteSize,
			int _indicesByteSize,
			int _vertexSize,
			int _normalOffset,
			int _texCoordOffset
		) noexcept;
		void setClearColour(float _r, float _g, float _b) noexcept;
		void setRenderMode(const int _mode) noexcept;
		void setResolution(
			const size_t _width,
			const size_t _height
		) noexcept;

	}
}
