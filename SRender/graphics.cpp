#include "graphics.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace srender
{
	namespace graphics
	{
		bool l_gladLoaded = false;

		bool loadGlad() noexcept
		{
			// Glad: load all OpenGL function pointers
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{	return false; }

			l_gladLoaded = true;

			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_STENCIL_TEST);

			return true;
		}

		bool getGladLoaded() noexcept
		{	return l_gladLoaded; }

		void clearScreenBuffers() noexcept
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void setClearColour(
			const float _r,
			const float _g,
			const float _b,
			const float _a
		) noexcept
		{	glClearColor((GLfloat)_r, (GLfloat)_g, (GLfloat)_b, (GLfloat)_a); }

		void setRenderMode(const int _mode) noexcept
		{	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + _mode); }

		void setResolution(const size_t _width, const size_t _height) noexcept
		{	glViewport(0, 0, (GLsizei)_width, (GLsizei)_height); }

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
		) noexcept
		{
			// Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
			// Arguments are number of objects to generate, and an array of uints to have the ids stored in
			glGenVertexArrays(1, _outIdVAO);
			glGenBuffers(1, _outIdVBO);
			glGenBuffers(1, _outIdEBO);

			// Binds the vertex array so that the VBO and EBO are neatly stored within
			glBindVertexArray(*_outIdVAO);

			// GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
			glBindBuffer(GL_ARRAY_BUFFER, *_outIdVBO);
			// Loads the vertices to the VBO
			glBufferData(GL_ARRAY_BUFFER, (GLsizei)_verticesByteSize, _vertices, GL_STATIC_DRAW);

			/*GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			* GL_STATIC_DRAW: the data is set only once and used many times.
			*GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/

			// This buffer stores the indices that reference the elements of the VBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_outIdEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)_indicesByteSize, _indices, GL_STATIC_DRAW);

			/*Tells the shader how to use the vertex data provided
			* p1: Which vertex attribute we want to configure in the vertex shader (location = 0)
			* p2: Vertex size (vec3)
			* p3: The type of data (vec is using floats)
			* p4: Whether we want to normalise the data
			* p5: Stride, how big each chunk of data is
			* p6: Offset, for some reason a void*
			*/
			// Position attribute
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _vertexSize, (void*)0);
			// Normal attribute
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _vertexSize, (void*)_normalOffset);
			// Texcoord attribute
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, _vertexSize, (void*)_texCoordOffset);

			// Unbinds the vertex array
			glBindVertexArray(0);
			// Unbinds the GL_ARRAY_BUFFER
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Unbinds the GL_ELEMENT_ARRAY_BUFFER
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void deleteMesh(
			const uint32_t _idVAO,
			const uint32_t _idVBO,
			const uint32_t _idEBO
		) noexcept
		{
			if (l_gladLoaded)
			{
				glDeleteVertexArrays(1, &_idVAO);
				glDeleteBuffers(1, &_idVBO);
				glDeleteBuffers(1, &_idEBO);
			}
		}

		void drawElements(const uint32_t _idVAO, const uint32_t _size) noexcept
		{
			glBindVertexArray(_idVAO);
			glDrawElements(
				GL_TRIANGLES,
				(GLsizei)_size,
				GL_UNSIGNED_INT,
				0
			);
			glBindVertexArray(0);
		}

		// Texture

		void setActiveTexture(uint8_t _num) noexcept
		{
			if (_num > 31)
			{	return; }

			glActiveTexture(GL_TEXTURE0 + _num);
		}

		void genTexture(uint32_t *_idTex) noexcept
		{	glGenTextures(1, _idTex); }

		void bindTexture2D(uint32_t _idTex) noexcept
		{	glBindTexture(GL_TEXTURE_2D, _idTex); }

		void setBorderColour(float *_arr) noexcept
		{	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _arr); }

		void setTex2DParamSWrapToEdge() noexcept
		{	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); }

		void setTex2DParamTWrapTOBorder() noexcept
		{	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); }

		void setTex2DParamMinFilterLinearMipMapLinear() noexcept
		{	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); }

		void setTex2DParamMagFilterNearest() noexcept
		{	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); }

		void loadTexture(
			int _width,
			int _height,
			int _numComponents,
			unsigned char *_imageData
		) noexcept
		{
			GLenum format;
			switch (_numComponents)
			{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
				default: return;
			}

			/*Applies the image to the texture object and creates the mipmaps
			* p1: What object we are applying to
			* p2: Specifies which mipmap level we are applying to (0 for base)
			* p3: What format to store the texture as
			* p4/5: The width and height of the texture
			* p6: Border (legacy stuff, leave as 0)
			* p7: What format the image is
			* p8: The datatype being passed in (in this case a char)
			* p9: The image data being passed in
			*/
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, format, GL_UNSIGNED_BYTE, _imageData);
		}

		void genMipmap() noexcept
		{	glGenerateMipmap(GL_TEXTURE_2D); }

		void deleteTextures(const uint32_t _textureCount, const uint32_t *_textureIds) noexcept
		{
			if (l_gladLoaded)
			{	glDeleteTextures(_textureCount, _textureIds); }
		}

		// Shader

		uint32_t createShaderProgram(uint32_t _idVertex, uint32_t _idFragment) noexcept
		{
			uint32_t idProgram = glCreateProgram();
			// Link the vertex and fragment shaders
			glAttachShader(idProgram, _idVertex);
			glAttachShader(idProgram, _idFragment);
			glLinkProgram(idProgram);
			return idProgram;
		}

		uint32_t createVertexShader() noexcept
		{	return glCreateShader(GL_VERTEX_SHADER); }

		uint32_t createFragmentShader() noexcept
		{	return glCreateShader(GL_FRAGMENT_SHADER); }

		void useShaderProgram(uint32_t _idProgram) noexcept
		{	glUseProgram(_idProgram); }

		void loadShaderSource(uint32_t _idShader, const char *_code) noexcept
		{	glShaderSource(_idShader, 1, &_code, NULL); }

		void compileShader(uint32_t _idShader) noexcept
		{	glCompileShader(_idShader); }

		void getProgramiv(uint32_t _idShader, int32_t *_success) noexcept
		{
			glGetProgramiv(_idShader, GL_LINK_STATUS, _success);
		}

		void getProgramInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept
		{
			glGetProgramInfoLog(_idShader, _logSize, NULL, _infoLog);
		}

		void getShaderiv(uint32_t _idShader, int32_t *_success) noexcept
		{
			glGetShaderiv(_idShader, GL_COMPILE_STATUS, _success);
		}

		void getShaderInfoLog(uint32_t _idShader, char *_infoLog, uint16_t _logSize) noexcept
		{
			glGetShaderInfoLog(_idShader, _logSize, NULL, _infoLog);
		}

		void deleteShaderProgram(const uint32_t _idProgram) noexcept
		{
			if (l_gladLoaded)
			{	glDeleteProgram(_idProgram); }
		}

		void deleteShader(const uint32_t _idShader) noexcept
		{	glDeleteShader(_idShader); }

		int32_t getUniformLocation(uint32_t _idProgram, const char *_name) noexcept
		{	return glGetUniformLocation(_idProgram, _name); }

		void setBool(uint32_t _idProgram, int32_t _location, const bool _value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform1i(_location, (int32_t)_value);
		}

		void setInt(uint32_t _idProgram, int32_t _location, const int32_t _value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform1i(_location, (int32_t)_value);
		}

		void setUint(uint32_t _idProgram, int32_t _location, const uint32_t _value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform1ui(_location, (int32_t)_value);
		}

		void setFloat(uint32_t _idProgram, int32_t _location, const float _value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform1f(_location, _value);
		}

		void setFloat2(uint32_t _idProgram, int32_t _location, const float *_value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform2fv(_location, 1, _value);
		}

		void setFloat3(uint32_t _idProgram, int32_t _location, const float *_value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform3fv(_location, 1, _value);
		}

		void setFloat4(uint32_t _idProgram, int32_t _location, const float *_value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniform4fv(_location, 1, _value);
		}

		void setMat3(uint32_t _idProgram, int32_t _location, const float *_value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniformMatrix3fv(_location, 1, GL_FALSE, _value);
		}

		void setMat4(uint32_t _idProgram, int32_t _location, const float *_value) noexcept
		{
			useShaderProgram(_idProgram);
			glUniformMatrix4fv(_location, 1, GL_FALSE, _value);
		}
	}
}
