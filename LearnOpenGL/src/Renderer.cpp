#pragma region
#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#ifdef _DEBUG
 #include <iostream>
#endif

#pragma endregion

namespace Engine
{
	void Renderer::Init(float pAspect)
	{
		// Initialise camera
		m_cameraRef = new Camera(pAspect, 75.0f);
		m_cameraRef->SetClearColour(0.1f, 0.1f, 0.1f);
		m_cameraRef->SetPosition({ 0.0f, 0.0f, 6.0f });

		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);

		// Mesh creation
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_meshes.get()->push_back(make_unique<Mesh>(0));

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

		GetTextureAt(0U)->LoadImages();
		//GetShaderAt(0U)->Use();
		//GetShaderAt(0U)->SetInt("texture0", 0);
		//GetShaderAt(0U)->SetInt("texture1", 1);
		//GetShaderAt(0U)->SetInt("texture2", 2);

		GetShaderAt(0U)->SetInt("u_material.diffuse", 0);
		GetShaderAt(0U)->SetInt("u_material.specular", 1);
		GetShaderAt(0U)->SetFloat("u_material.shininess", 32.0f);

		// Lights
		// Directional
		m_lightDirectional = new Light(LightType::Directional, vec3(0, -1, 0), vec3(0.8f));
		GetShaderAt(0U)->SetVec3("u_directional.colour.ambient", m_lightDirectional->GetColour() * 0.15f);
		GetShaderAt(0U)->SetVec3("u_directional.colour.diffuse", m_lightDirectional->GetColour());
		GetShaderAt(0U)->SetVec3("u_directional.colour.specular", m_lightDirectional->GetColour());
		GetShaderAt(0U)->SetVec4("u_directional.direction", m_lightDirectional->GetDirection());

		// Point
		m_lightPoint = new Light(LightType::Point, vec4(-4, 2, -2, 1), vec3(1.0f));
		//GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.ambient", m_lightPoint->GetColour() * 0.15f);
		GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.diffuse", m_lightPoint->GetColour());
		GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.specular", m_lightPoint->GetColour());
		GetShaderAt(0U)->SetVec4("u_pointLights[0].position", m_lightPoint->GetPosition());
		GetShaderAt(0U)->SetFloat("u_pointLights[0].linear", 0.045f);
		GetShaderAt(0U)->SetFloat("u_pointLights[0].quadratic", 0.0075f);

		//GetShaderAt(0U)->SetUint("u_light.type", (unsigned int)m_lightPoint->GetType());
		//GetShaderAt(0U)->SetVec3("u_light.colour.ambient", m_lightPoint->GetColour() * 0.15f);
		//GetShaderAt(0U)->SetVec3("u_light.colour.diffuse", m_lightPoint->GetColour());
		//GetShaderAt(0U)->SetVec3("u_light.colour.specular", m_lightPoint->GetColour());
		//GetShaderAt(0U)->SetVec4("u_light.position", m_lightPoint->GetPosition());
		//GetShaderAt(0U)->SetFloat("u_light.linear", 0.045f);
		//GetShaderAt(0U)->SetFloat("u_light.quadratic", 0.0075f);

		// Spot
		m_lightSpot = new Light(LightType::Spot, vec4(4.5f, 3, 3.5f, 1), vec3(-0.7f, -0.6f, -1), vec3(1.0f), 17.0f, 0.2f);
		//GetShaderAt(0U)->SetVec3("u_spotLights[0].colour.ambient", m_lightSpot->GetColour() * 0.15f);
		GetShaderAt(0U)->SetVec3("u_spotLights[0].colour.diffuse", m_lightSpot->GetColour());
		GetShaderAt(0U)->SetVec3("u_spotLights[0].colour.specular", m_lightSpot->GetColour());
		GetShaderAt(0U)->SetVec4("u_spotLights[0].position", m_lightSpot->GetPosition());
		GetShaderAt(0U)->SetVec4("u_spotLights[0].direction", m_lightSpot->GetDirection());
		GetShaderAt(0U)->SetFloat("u_spotLights[0].linear", 0.045f);
		GetShaderAt(0U)->SetFloat("u_spotLights[0].quadratic", 0.0075f);
		GetShaderAt(0U)->SetFloat("u_spotLights[0].cutoff", m_lightSpot->GetAngle());
		GetShaderAt(0U)->SetFloat("u_spotLights[0].blur", m_lightSpot->GetBlur());

		// Point light cube
		{
			// Light cube vertex data
			m_meshes.get()->push_back(make_unique<Mesh>(1));
			CreateLightVAO(GetMeshAt(1U)->GetVAO(),
				GetMeshAt(1U)->GetVBO(),
				GetMeshAt(1U)->GetVertices());

			// Light cube shader
			m_shaders.get()->push_back(make_unique<Shader>("../Assets/shaders/light.vert", "../Assets/shaders/light.frag"));
			GetShaderAt(1U)->SetVec3("u_colour", m_lightPoint->GetColour());

			mat4 lightModel = mat4(1.0f);
			lightModel = glm::translate(lightModel, vec3(m_lightPoint->GetPosition()));
			GetShaderAt(1U)->SetMat4("u_model", (mat4)lightModel);
		}

		// Spot light cube
		{
			// Light cube vertex data
			m_meshes.get()->push_back(make_unique<Mesh>(1));
			CreateLightVAO(GetMeshAt(2U)->GetVAO(),
				GetMeshAt(2U)->GetVBO(),
				GetMeshAt(2U)->GetVertices());

			// Light cube shader
			m_shaders.get()->push_back(make_unique<Shader>("../Assets/shaders/light.vert", "../Assets/shaders/light.frag"));
			GetShaderAt(2U)->SetVec3("u_colour", m_lightSpot->GetColour());

			mat4 lightModel = mat4(1.0f);
			lightModel = glm::translate(lightModel, vec3(m_lightSpot->GetPosition()));
			GetShaderAt(2U)->SetMat4("u_model", (mat4)lightModel);
		}

		// #ifdef _DEBUG
		//  if (m_shaders.get() != nullptr)
		//  	std::cout << "Shader loaded: " << (GetShaderAt(0U)->GetLoaded() ? "True " : "False ");
		//
		//  if (m_textures.get() != nullptr)
		//  	std::cout << "Texture loaded: " << (GetTextureAt(0U)->GetLoaded() ? "True" : "False");
		//
		//  std::cout << std::endl;
		// #endif
	}

	void Renderer::Destroy(bool pValidate)
	{
		if (pValidate)
		{
			// Destroy all meshes
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

			// Destroy all shaders
			for (unsigned int i = 0; i < (*m_shaders.get()).size(); ++i)
			{
				if (GetShaderAt(i) != nullptr)
					GetShaderAt(i)->Destroy(pValidate);
			}
			
			// Destroy all textures
			for (unsigned int i = 0; i < (*m_textures.get()).size(); ++i)
			{
				if (GetTextureAt(i) != nullptr)
					GetTextureAt(i)->Destroy(pValidate);
			}
		}

		// Tell the unique pointers they are no longer needed
		m_meshes.release();
		m_shaders.release();
		m_textures.release();

		delete m_cameraRef;
	}

	void Renderer::Draw(double pTime)
	{
		// Rendering

		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Point light cube
		{
			// Light cube rendering
			glBindVertexArray(*GetMeshAt(1U)->GetVAO());
			GetShaderAt(1U)->Use();
			GetShaderAt(1U)->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Spot light cube
		{
			// Light cube rendering
			glBindVertexArray(*GetMeshAt(2U)->GetVAO());
			GetShaderAt(2U)->Use();
			GetShaderAt(2U)->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Must be set to the current context
		glBindVertexArray(*GetMeshAt(0U)->GetVAO());
		GetShaderAt(0U)->Use();
		GetShaderAt(0U)->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
		GetShaderAt(0U)->SetVec3("u_viewPos", m_cameraRef->GetPosition());

		for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = mat4(1.0f);
			model = glm::translate(model, m_cubePositions[i]);
			float angle = (float)pTime * 5.0f * ((i + 1) / (i * 0.2f + 1));
			model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
			GetShaderAt(0U)->SetMat4("u_model", (mat4)model);
			mat3 transposeInverseOfModel = mat3(glm::transpose(glm::inverse(model)));
			GetShaderAt(0U)->SetMat3("u_transposeInverseOfModel", (mat3)transposeInverseOfModel);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
		// Normal attribute
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

	void Renderer::CreateLightVAO(unsigned int* pidVAO, unsigned int* pidVBO, float* pVertices[], const size_t pVertexSize)
	{
		glGenVertexArrays(1, pidVAO);
		glGenBuffers(1, pidVBO);
		glBindVertexArray(*pidVAO);
		glBindBuffer(GL_ARRAY_BUFFER, *pidVBO);
		glBufferData(GL_ARRAY_BUFFER, pVertexSize, *pVertices, GL_STATIC_DRAW);

		/*Tells the shader how to use the vertex data provided
		* p1: Which vertex attribute we want to configure in the vertex shader (location = 0)
		* p2: Vertex size (vec3)
		* p3: The type of data (vec is using floats)
		* p4: Whether we want to normalise the data
		* p5: Stride, because the data is tightly packed this can be 0
		* p6: Offset, for some reason a void*
		*/
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
		// // Colour attribute
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		// glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Renderer::CreateLightVAO(unsigned int* pidVAO, unsigned int* pidVBO, vector<float>* pVertices)
	{
		const size_t vertexSize = pVertices->size();

		float* vertices = new float[vertexSize];
		vertices = pVertices->data();

		CreateLightVAO(pidVAO, pidVBO, &vertices, vertexSize * sizeof(float));

		// Pointer is not automatically deleted at end of scope
		delete[] vertices;
	}

	#pragma region Getters
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
	#pragma endregion
}
