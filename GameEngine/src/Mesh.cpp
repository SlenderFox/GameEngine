#pragma region
#include "Mesh.hpp"
#include "glad/glad.h"
#include <assert.h>

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
#pragma endregion

namespace Engine
{
	// Forward declaration
	class Application { public: static const bool GladLoaded() noexcept; };

	//Static
	const float* Mesh::s_cubeVerticesArr = new float[288] {
		// Positions		   // Normals			  // Texture coords
	    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
	};

	const uint32_t* Mesh::s_indicesArr = new uint32_t[36] {
		0U,	1U,	2U,	3U,	4U,	5U,	// Face 1
		6U, 	7U,	8U,	9U,	10U,	11U,	// Face 2
		12U,	13U,	14U,	15U,	16U,	17U,	// Face 3
		18U,	19U,	20U,	21U,	22U,	23U,	// Face 4
		24U,	25U,	26U,	27U,	28U,	29U,	// Face 5
		30U,	31U,	32U,	33U,	34U,	35U	// Face 6
	};

	vector<Vertex> Mesh::GenerateVertices() noexcept
	{
		vector<Vertex> verts = vector<Vertex>();

		// Makes cube with pos, normal, and texcoord
		for (uint8_t i = 0; i < 36; ++i)
		{
			Vertex vert;
			for (uint8_t j = 0; j < 8; ++j)
			{
				switch (j)
				{
				case 0: vert.position.x  = s_cubeVerticesArr[i * 8 + j]; break;
				case 1: vert.position.y  = s_cubeVerticesArr[i * 8 + j]; break;
				case 2: vert.position.z  = s_cubeVerticesArr[i * 8 + j]; break;
				case 3: vert.normal.x    = s_cubeVerticesArr[i * 8 + j]; break;
				case 4: vert.normal.y    = s_cubeVerticesArr[i * 8 + j]; break;
				case 5: vert.normal.z    = s_cubeVerticesArr[i * 8 + j]; break;
				case 6: vert.texCoords.x = s_cubeVerticesArr[i * 8 + j]; break;
				case 7: vert.texCoords.y = s_cubeVerticesArr[i * 8 + j]; break;
				}
			}
			verts.push_back(vert);
		}
		verts.shrink_to_fit();
		return verts;
	}

	vector<uint32_t> Mesh::GenerateIndices() noexcept
	{
		vector<uint32_t> inds = vector<uint32_t>();

		for (uint8_t i = 0; i < 36; ++i)
			inds.push_back(s_indicesArr[i]);
		inds.shrink_to_fit();
		return inds;
	}

	//Member
	Mesh::Mesh()
	{
		m_vertices = make_unique<vector<Vertex>>(GenerateVertices());
		m_indices = make_unique<vector<uint32_t>>(GenerateIndices());

		SetupMesh();
	}

	Mesh::Mesh(
		const std::vector<Vertex>* pVertices,
		const std::vector<uint32_t>* pIndices)
	{
		m_vertices = make_unique<vector<Vertex>>(*pVertices);
		m_indices = make_unique<vector<uint32_t>>(*pIndices);
		SetupMesh();
	}

	Mesh::~Mesh()
	{
		if (Application::GladLoaded())
		{
			glDeleteVertexArrays(1, m_idVAO);
			glDeleteBuffers(1, m_idVBO);
			glDeleteBuffers(1, m_idEBO);
		}

		m_vertices.get()->clear();
		m_vertices.release();
		m_indices.get()->clear();
		m_indices.release();
		delete m_idVAO;
		delete m_idVBO;
		delete m_idEBO;
	}

	void Mesh::Draw() const noexcept
	{
		glBindVertexArray(*m_idVAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.get()->size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::SetupMesh() const noexcept
	{
		// Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
		// Arguments are number of objects to generate, and an array of uints to have the ids stored in
		glGenVertexArrays(1, m_idVAO);
		glGenBuffers(1, m_idVBO);
		glGenBuffers(1, m_idEBO);

		// Binds the vertex array so that the VBO and EBO are neatly stored within
		glBindVertexArray(*m_idVAO);

		// GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
		glBindBuffer(GL_ARRAY_BUFFER, *m_idVBO);
		// Loads the vertices to the VBO
		glBufferData(
			GL_ARRAY_BUFFER,
			(GLsizei)m_vertices.get()->size() * sizeof(Vertex),
			&(*m_vertices.get())[0],	// Returns the zeroth element in the vector
			GL_STATIC_DRAW
		);

		/*GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		* GL_STATIC_DRAW: the data is set only once and used many times.
		*GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
		*/

		// This buffer stores the indices that reference the elements of the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_idEBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			(GLsizei)m_indices.get()->size() * sizeof(uint32_t),
			&(*m_indices.get())[0],	// Returns the zeroth element in the vector
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// Texcoord attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		// Unbinds the vertex array
		glBindVertexArray(0);
		// Unbinds the GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbinds the GL_ELEMENT_ARRAY_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
