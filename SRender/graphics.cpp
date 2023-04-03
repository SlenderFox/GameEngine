#include "graphics.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace srender
{
	namespace graphics
	{
		bool l_gladLoaded = false;

		const bool loadGlad() noexcept
		{
			// Glad: load all OpenGL function pointers
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{	return false; }

			l_gladLoaded = true;

			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_STENCIL_TEST);

			return true;
		}

		const bool getGladLoaded() noexcept
		{	return l_gladLoaded; }

		void clear() noexcept
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		void deleteShader(const uint32_t _idProgram) noexcept
		{
			if (l_gladLoaded)
			{	glDeleteProgram(_idProgram); }
		}

		void deleteTextures(const uint32_t _textureCount, const uint32_t *_textureIds) noexcept
		{
			if (l_gladLoaded)
			{	glDeleteTextures(_textureCount, _textureIds); }
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
	}
}
