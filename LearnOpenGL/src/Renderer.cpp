#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#ifdef _DEBUG
 #include <iostream>
#endif

using glm::vec3;
using glm::mat4;

namespace Engine
{
	void Renderer::Init()
	{
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);

		// Mesh creation
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_meshes.get()->push_back(make_unique<Mesh>());

		CreateVAO(GetMeshAt(0U)->GetVAO(),
				  GetMeshAt(0U)->GetVBO(),
				  GetMeshAt(0U)->GetEBO(),
			  GetMeshAt(0U)->GetVertices(),
			  GetMeshAt(0U)->GetIndices());

		// Shader creation
		m_shaders = make_unique<vector<unique_ptr<Shader>>>();
		m_shaders.get()->push_back(make_unique<Shader>("../Assets/shaders/cube.vert", "../Assets/shaders/cube.frag"));

		// Texture creation
		m_textures = make_unique<vector<unique_ptr<Texture>>>();
		m_textures.get()->push_back(make_unique<Texture>());

		//GetShaderAt(0U)->LoadPaths("../Assets/shaders/cube.vert", "../Assets/shaders/cube.frag");
		GetTextureAt(0U)->LoadImages();
		//GetShaderAt(0U)->Use();
		GetShaderAt(0U)->SetInt("texture0", 0);
		GetShaderAt(0U)->SetInt("texture1", 1);
		GetShaderAt(0U)->SetInt("texture2", 2);

#ifdef _DEBUG
		if (m_shaders.get() != nullptr)
			std::cout << "Shader loaded: " << (GetShaderAt(0U)->GetLoaded() ? "True " : "False ");

		if (m_textures.get() != nullptr)
			std::cout << "Texture loaded: " << (GetTextureAt(0U)->GetLoaded() ? "True" : "False");

		std::cout << std::endl;
#endif

		// // Old way
		// m_shaderRef = new Shader("../Assets/shaders/cube.vert", "../Assets/shaders/cube.frag");
		// m_textureRef = new Texture();
		// m_textureRef->LoadImages();
		// //m_shaderRef->Use();
		// m_shaderRef->SetInt("texture0", 0);
		// m_shaderRef->SetInt("texture1", 1);
		// m_shaderRef->SetInt("texture2", 2);
	}

	void Renderer::Destroy(bool pValidate)
	{
		if (pValidate)
		{
			for (unsigned int i = 0; i < (*m_meshes.get()).size(); ++i)
			{
				if (GetMeshAt(i) != nullptr)
				{
					glDeleteVertexArrays(1, GetMeshAt(i)->GetVAO());
					glDeleteBuffers(1, GetMeshAt(i)->GetVBO());
					glDeleteBuffers(1, GetMeshAt(i)->GetEBO());
					GetMeshAt(i)->Destroy(pValidate);
				}
			}

			// if (m_shaderRef != nullptr)
			// 	m_shaderRef->Destroy(pValidate);
			
			for (unsigned int i = 0; i < (*m_shaders.get()).size(); ++i)
			{
				if (GetShaderAt(i) != nullptr)
					GetShaderAt(i)->Destroy(pValidate);
			}
			
			for (unsigned int i = 0; i < (*m_textures.get()).size(); ++i)
			{
				if (GetTextureAt(i) != nullptr)
					GetTextureAt(i)->Destroy(pValidate);
			}
		}

		m_meshes.release();
		m_shaders.release();
		m_textures.release();

		//delete m_meshes;
		//delete m_shaders;
		//delete m_textures;

		// delete m_shaderRef;
		// delete m_textureRef;
	}

	void Renderer::Draw(glm::mat4 pCamera, double pTime)
	{
		// Rendering

		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Must be set to the current context
		glBindVertexArray(*GetMeshAt(0U)->GetVAO());

		//m_shaderRef->Use();
		GetShaderAt(0U)->Use();

		//m_shaderRef->SetMat4("camera", pCamera);
		GetShaderAt(0U)->SetMat4("camera", pCamera);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = mat4(1.0f);
			model = glm::translate(model, m_cubePositions[i]);
			float angle = (float)pTime * 30.0f * ((i + 1) / (i * 0.2f + 1));
			model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
			//m_shaderRef->SetMat4("model", model);
			GetShaderAt(0U)->SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void Renderer::CreateVAO(unsigned int* pidVAO, unsigned int* pidVBO, unsigned int* pidEBO,
	 float* pVertices[], const size_t pVertexSize, unsigned int* pIndices[], const size_t pIndexSize)
	{
		// Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
		// Arguments are number of objects to generate, and an array of uints to have the ids stored in
		glGenVertexArrays(1, pidVAO);
		glGenBuffers(1, pidVBO);
		glGenBuffers(1, pidEBO);

		// Binds the vertex array so that the VBO and EBO are neatly stored within
		glBindVertexArray(*pidVAO);

		// GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
		glBindBuffer(GL_ARRAY_BUFFER, *pidVBO);
		// Loads the vertices to the VBO
		glBufferData(GL_ARRAY_BUFFER, pVertexSize, *pVertices, GL_STATIC_DRAW);

		/*GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		* GL_STATIC_DRAW: the data is set only once and used many times.
		*GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
		*/

		// This buffer stores the indices that reference the elements of the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pidEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndexSize, *pIndices, GL_STATIC_DRAW);

		/*Tells the shader how to use the vertex data provided
		* p1: Which vertex attribute we want to configure in the vertex shader (location = 0)
		* p2: Vertex size (vec3)
		* p3: The type of data (vec is using floats)
		* p4: Whether we want to normalise the data
		* p5: Stride, because the data is tightly packed this can be 0
		* p6: Offset, for some reason a void*
		*/
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Colour attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// Texcoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Unbinds the vertex array
		glBindVertexArray(0);
		// Unbinds the GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbinds the GL_ELEMENT_ARRAY_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Renderer::CreateVAO(unsigned int* pidVAO, unsigned int* pidVBO, unsigned int* pidEBO,
		 vector<float>* pVertices, vector<unsigned int>* pIndices)
	{
		const size_t vertexSize = pVertices->size();
		const size_t indexSize = pIndices->size();

		float* vertices = new float[vertexSize];
		vertices = pVertices->data();

		unsigned int* indices = new unsigned int[indexSize];
		indices = pIndices->data();

		CreateVAO(pidVAO, pidVBO, pidEBO, &vertices, vertexSize * sizeof(float), &indices, indexSize * sizeof(unsigned int));

		// Pointer is not automatically deleted at end of scope
		delete[] vertices;
		delete[] indices;
	}

	Mesh* Renderer::GetMeshAt(unsigned int pPos)
	{
		return (*m_meshes.get())[pPos].get();
	}

	Shader* Renderer::GetShaderAt(unsigned int pPos)
	{
		return (*m_shaders.get())[pPos].get();
	}

	Texture* Renderer::GetTextureAt(unsigned int pPos)
	{
		return (*m_textures.get())[pPos].get();
	}
}
