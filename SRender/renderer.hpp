/** Designed to be the one stop shop for swapping renderer libraries */
#include <cstdint>
#include <cstddef>

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** Interface for the rendering backend */
namespace renderer
{
	_NODISCARD bool loadGlad() noexcept;
	_NODISCARD bool getGladLoaded() noexcept;
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

	void setActiveTexture(uint8_t _num) noexcept;
	/** Generates a GL texture object
	 * @param _outIdTex OpenGL will place the texture id here
	 */
	void genTexture(uint32_t *_outIdTex) noexcept;
	void bindTexture2D(uint32_t _idTex) noexcept;
	void setBorderColour(float *_arr) noexcept;
	void setTex2DParamSWrapToEdge() noexcept;
	void setTex2DParamTWrapTOBorder() noexcept;
	void setTex2DParamMinFilterLinearMipMapLinear() noexcept;
	void setTex2DParamMagFilterNearest() noexcept;
	void loadTexture(
		int _width,
		int _height,
		int _numComponents,
		unsigned char *_imageData
	) noexcept;
	void genMipmap() noexcept;
	/** Delete one or multiple textures at once
	 * @param *_textureIds Pointer to the array of texture ids
	 * @param _textureCount The amount of textures to delete, defaults as 1
	 */
	void deleteTextures(const uint32_t *_textureIds, const uint32_t _textureCount = 1U) noexcept;

	// Shader

	_NODISCARD uint32_t createShaderProgram(uint32_t _idVertex, uint32_t _idFragment) noexcept;
	_NODISCARD uint32_t createVertexShader() noexcept;
	_NODISCARD uint32_t createFragmentShader() noexcept;
	void useShaderProgram(uint32_t _idProgram) noexcept;
	void loadShaderSource(uint32_t _idShader, const char *_code) noexcept;
	void compileShader(uint32_t _idShader) noexcept;
	void getProgramiv(uint32_t _idShader, int32_t *_success) noexcept;
	void getProgramInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept;
	void getShaderiv(uint32_t _idShader, int32_t *_success) noexcept;
	void getShaderInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept;
	void deleteShaderProgram(const uint32_t _idProgram) noexcept;
	void deleteShader(const uint32_t _idShader) noexcept;

	_NODISCARD int32_t getUniformLocation(uint32_t _idProgram, const char *_name) noexcept;
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
