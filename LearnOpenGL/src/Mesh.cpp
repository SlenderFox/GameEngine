#include "Mesh.hpp"
#include "glad/glad.h" // Include glad to get all the required OpenGL headers
#include <assert.h>

namespace Engine
{
	float* Mesh::s_cubeVerticesArr = new float[288] {
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

	unsigned int* Mesh::s_indicesArr = new unsigned int[36] {
		0U, 1U, 2U, 3U, 4U, 5U,			// Face 1
		6U, 7U, 8U,	9U, 10U, 11U,		// Face 2
		12U, 13U, 14U, 15U, 16U, 17U,	// Face 3
		18U, 19U, 20U, 21U, 22U, 23U,	// Face 4
		24U, 25U, 26U, 27U, 28U, 29U,	// Face 5
		30U, 31U, 32U, 33U, 34U, 35U	// Face 6
	};
	
	Mesh::Mesh()
	{
		m_vertices = make_unique<vector<Vertex>>(GenerateVertices());
		m_indices = make_unique<vector<unsigned int>>(GenerateIndices());
		m_textures = make_unique<vector<Texture>>();
		
		SetupMesh();
	}

	Mesh::Mesh(vector<Vertex> pVertices, vector<unsigned int> pIndices, vector<Texture> pTextures)
	{
		m_vertices = make_unique<vector<Vertex>>(pVertices);
		m_indices = make_unique<vector<unsigned int>>(pIndices);
		m_textures = make_unique<vector<Texture>>(pTextures);

		SetupMesh();
	}
	
	Mesh::Mesh(unique_ptr<vector<Vertex>> pVertices, unique_ptr<vector<unsigned int>> pIndices, unique_ptr<vector<Texture>> pTextures)
	{
		m_vertices = make_unique<vector<Vertex>>(*pVertices);
		m_indices = make_unique<vector<unsigned int>>(*pIndices);
		m_textures = make_unique<vector<Texture>>(*pTextures);
		
		SetupMesh();
	}
	
	// These are probably bad
	#pragma region Copy constructors
	Mesh::Mesh(const Mesh& pOther)
	{
		m_vertices = make_unique<vector<Vertex>>(*pOther.GetVertices());
		m_indices = make_unique<vector<unsigned int>>(*pOther.GetIndices());
		m_textures = make_unique<vector<Texture>>(*pOther.GetTextures());
	}

	Mesh::Mesh(Mesh&& pOther) noexcept
	{
		m_vertices = make_unique<vector<Vertex>>(*pOther.GetVertices());
		m_indices = make_unique<vector<unsigned int>>(*pOther.GetIndices());
		m_textures = make_unique<vector<Texture>>(*pOther.GetTextures());
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		Mesh* newObj = new Mesh(*pOther.GetVertices(), *pOther.GetIndices(), *pOther.GetTextures());
		return *newObj;
	}

	Mesh& Mesh::operator=(Mesh&& pOther) noexcept
	{
		Mesh* newObj = new Mesh(*pOther.GetVertices(), *pOther.GetIndices(), *pOther.GetTextures());
		return *newObj;
	}
	#pragma endregion

	void Mesh::Destroy(bool pValidate)
	{
		if (pValidate)
		{
			glDeleteVertexArrays(1, m_idVAO);
			glDeleteBuffers(1, m_idVBO);
			glDeleteBuffers(1, m_idEBO);
		}

		GetVertices()->clear();
		m_vertices.release();
		GetIndices()->clear();
		m_indices.release();
		GetTextures()->clear();
		m_textures.release();
		delete m_idVAO;
		delete m_idVBO;
		delete m_idEBO;
	}
	
	void Mesh::LoadTextures(Shader& pShader)
	{
		unsigned int diffuseNr = 0;
		unsigned int specularNr = 0;
		for (unsigned int i = 0; i < GetTextures()->size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = GetTextures()->at(i).GetType();
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			pShader.SetInt(("u_material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, GetTextures()->at(i).GetId());
		}
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::Draw(Shader* pShader)
	{
		// Draw mesh
		glBindVertexArray(*m_idVAO);
		glDrawElements(GL_TRIANGLES, GetIndices()->size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// Static
	vector<Vertex> Mesh::GenerateVertices()
	{
		vector<Vertex> verts = vector<Vertex>();

		// Makes cube with pos, normal, and texcoord
		for (int i = 0; i < 36; ++i)
		{
			Vertex vert;
			for (int j = 0; j < 8; ++j)
			{
				switch (j)
				{
					case 0: vert.position.x  = vert.position.x  = s_cubeVerticesArr[i * 8 + j]; break;
					case 1: vert.position.y  = vert.position.y  = s_cubeVerticesArr[i * 8 + j]; break;
					case 2: vert.position.z  = vert.position.z  = s_cubeVerticesArr[i * 8 + j]; break;
					case 3: vert.normal.x    = vert.normal.x    = s_cubeVerticesArr[i * 8 + j]; break;
					case 4: vert.normal.y    = vert.normal.y    = s_cubeVerticesArr[i * 8 + j]; break;
					case 5: vert.normal.z    = vert.normal.z    = s_cubeVerticesArr[i * 8 + j]; break;
					case 6: vert.texCoords.x = vert.texCoords.x = s_cubeVerticesArr[i * 8 + j]; break;
					case 7: vert.texCoords.y = vert.texCoords.y = s_cubeVerticesArr[i * 8 + j]; break;
				}
			}
			verts.push_back(vert);
		}
		verts.shrink_to_fit();
		return verts;
	}

	// Static
	vector<unsigned int> Mesh::GenerateIndices()
	{
		vector<unsigned int> inds = vector<unsigned int>();

		for (int i = 0; i < 36; ++i)
			inds.push_back(s_indicesArr[i]);
		inds.shrink_to_fit();
		return inds;
	}

	void Mesh::SetupMesh()
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
		glBufferData(GL_ARRAY_BUFFER, GetVertices()->size() * sizeof(Vertex), &(*GetVertices())[0], GL_STATIC_DRAW);

		/*GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		* GL_STATIC_DRAW: the data is set only once and used many times.
		*GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
		*/

		// This buffer stores the indices that reference the elements of the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_idEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndices()->size() * sizeof(unsigned int), &(*GetIndices())[0], GL_STATIC_DRAW);

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

	#pragma region Setters
	void Mesh::SetVertices(vector<Vertex>* pVertices)
	{
		m_vertices = make_unique<vector<Vertex>>(*pVertices);
	}

	void Mesh::SetIndices(vector<unsigned int>* pIndices)
	{
		m_indices = make_unique<vector<unsigned int>>(*pIndices);
	}

	void Mesh::SetTextures(vector<Texture>* pTextures)
	{
		m_textures = make_unique<vector<Texture>>(*pTextures);
	}
	#pragma endregion
	#pragma region Getters
	vector<Vertex>* Mesh::GetVertices() const
	{
		return m_vertices.get();
	}
	
	vector<unsigned int>* Mesh::GetIndices() const
	{
		return m_indices.get();
	}

	vector<Texture>* Mesh::GetTextures() const
	{
		return m_textures.get();
	}

	unsigned int* Mesh::GetVAO() const
	{
		return m_idVAO;
	}

	unsigned int* Mesh::GetVBO() const
	{
		return m_idVBO;
	}

	unsigned int* Mesh::GetEBO() const
	{
		return m_idEBO;
	}
	#pragma endregion
}
