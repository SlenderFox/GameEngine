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
			l_gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (!l_gladLoaded)
			{ return false; }

			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_STENCIL_TEST);

			return true;
		}

		const bool gladLoaded() noexcept
		{
			return l_gladLoaded;
		}

		void clear() noexcept
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void deleteMesh(uint32_t _idVAO, uint32_t _idVBO, uint32_t _idEBO) noexcept
		{
			if (l_gladLoaded)
			{
				glDeleteVertexArrays(1, &_idVAO);
				glDeleteBuffers(1, &_idVBO);
				glDeleteBuffers(1, &_idEBO);
			}
		}

		void deleteShader(uint32_t _idProgram) noexcept
		{
			if (l_gladLoaded)
			{ glDeleteProgram(_idProgram); }
		}

		void deleteTextures(uint32_t _textureCount, uint32_t *_textureIds) noexcept
		{
			if (l_gladLoaded)
			{ glDeleteTextures(_textureCount, _textureIds); }
		}

		void drawElements(uint32_t _idVAO, std::vector<uint32_t> *_indices) noexcept
		{
			glBindVertexArray(_idVAO);
			glDrawElements(
				GL_TRIANGLES,
				(GLsizei)_indices->size(),
				GL_UNSIGNED_INT,
				0
			);
			glBindVertexArray(0);
		}

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
		) noexcept
		{
			// Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
			// Arguments are number of objects to generate, and an array of uints to have the ids stored in
			glGenVertexArrays(1, _idVAO);
			glGenBuffers(1, _idVBO);
			glGenBuffers(1, _idEBO);

			// Binds the vertex array so that the VBO and EBO are neatly stored within
			glBindVertexArray(*_idVAO);

			// GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
			glBindBuffer(GL_ARRAY_BUFFER, *_idVBO);
			// Loads the vertices to the VBO
			glBufferData(
				GL_ARRAY_BUFFER,
				(GLsizei)_verticesByteSize,
				_vertices,
				GL_STATIC_DRAW
			);

			/*GL_STREA_DRAW: the data is set only once and used by the GPU at most a few times.
			* GL_STATIC_DRAW: the data is set only once and used many times.
			*GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			*/

			// This buffer stores the indices that reference the elements of the VBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_idEBO);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				(GLsizei)_indicesByteSize,
				_indices,
				GL_STATIC_DRAW
			);

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

		void setClearColour(float _r, float _g, float _b) noexcept
		{
			glClearColor((GLfloat)_r, (GLfloat)_g, (GLfloat)_b, 1.0f);
		}

		void setRenderMode(const int _mode) noexcept
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + _mode);
		}

		void setResolution(
			const size_t _width,
			const size_t _height
		) noexcept
		{
			glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
		}
	}
}
