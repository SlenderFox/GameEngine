#pragma region
#include "Renderer.hpp"
#include "glad/glad.h" // Include glad to get all the required OpenGL headers
#include "glm/gtc/matrix_transform.hpp"
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
// #define LOCATION "../"
//#else
// #define LOCATION ""
#endif

#pragma endregion

namespace Engine
{
	void Renderer::Init(float pAspect)
	{
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);

		// Initialise camera
		m_cameraRef = new Camera(pAspect, 75.0f);
		m_cameraRef->SetClearColour(0.1f, 0.1f, 0.1f);
		m_cameraRef->SetPosition({ 0.0f, 0.0f, 6.0f });

		// Initialise lights
		m_lightDirectional = new Light(LightType::Directional, vec3(0, -1, 0), vec3(0.8f));
		m_lightPoint = new Light(LightType::Point, vec4(-4, 2, -2, 1), vec3(1.0f));
		m_lightSpot = new Light(LightType::Spot, vec4(4.5f, 3, 3.5f, 1), vec3(-0.7f, -0.6f, -1), vec3(1.0f), 17.0f, 0.2f);

		// Initialise vectors
		m_shaders = make_unique<vector<unique_ptr<Shader>>>();
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();

		CreateBoxScene();

		// #ifdef _DEBUG
		//  if (m_meshes.get() != nullptr)
		//  	cout << "Meshes loaded: " << (GetMeshAt(0U) ? "True " : "False ");
		//  if (m_shaders.get() != nullptr)
		//  	cout << "Shader loaded: " << (GetShaderAt(0U)->GetLoaded() ? "True" : "False");
		//  cout << endl;
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
					GetMeshAt(i)->Destroy(pValidate);
				}
			}

			// Destroy all shaders
			for (unsigned int i = 0; i < (*m_shaders.get()).size(); ++i)
			{
				if (GetShaderAt(i) != nullptr)
					GetShaderAt(i)->Destroy(pValidate);
			}
			
			// Unload all textures from memory once finished
			Texture::UnloadAll(pValidate);
		}

		// Tell the unique pointers they are no longer needed
		m_meshes.release();
		m_shaders.release();

		delete m_cameraRef;
	}

	void Renderer::Draw(double pTime)
	{
		// Rendering

		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < m_meshes.get()->size(); ++i)
		{
			glBindVertexArray(*GetMeshAt(i)->GetVAO());
			GetShaderAt(i)->Use();
			GetShaderAt(i)->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			GetShaderAt(i)->SetVec3("u_viewPos", m_cameraRef->GetPosition());
			if (i > 0)
			{
				//glDrawElements(GL_TRIANGLES, GetMeshAt(i)->GetIndices()->size(), GL_UNSIGNED_INT, 0);
				GetMeshAt(i)->Draw(*GetShaderAt(i));
			}
			else
			{
				for (unsigned int j = 0; j < 10; j++)
				{
					mat4 model = mat4(1.0f);
					model = glm::translate(model, m_cubePositions[j]);
					float angle = (float)pTime * 5.0f * ((j) / (j * 0.2f + 1));
					model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
					GetShaderAt(0U)->SetMat4("u_model", (mat4)model);
					mat3 transposeInverseOfModel = mat3(glm::transpose(glm::inverse(model)));
					GetShaderAt(0U)->SetMat3("u_transposeInverseOfModel", (mat3)transposeInverseOfModel);
					//glDrawElements(GL_TRIANGLES, GetMeshAt(i)->GetIndices()->size(), GL_UNSIGNED_INT, 0);
					GetMeshAt(i)->Draw(*GetShaderAt(i));
				}
			}
		}
	}

	void Renderer::CreateBoxScene()
	{
		m_shaders.get()->push_back(make_unique<Shader>("assets/shaders/cube"));

		vector<Texture> textures = vector<Texture>();
		textures.push_back(Texture("assets/textures/container2.png", TexType::diffuse));
		textures.push_back(Texture("assets/textures/container2_specular.png", TexType::specular));

		m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices(), textures));

		GetMeshAt(0U)->LoadTextures(*GetShaderAt(0U));
		GetShaderAt(0U)->SetFloat("u_material.shininess", 32.0f);

		#pragma region Lights
		 // Directional
		 GetShaderAt(0U)->SetVec3("u_directional.colour.ambient", m_lightDirectional->GetColour() * 0.15f);
		 GetShaderAt(0U)->SetVec3("u_directional.colour.diffuse", m_lightDirectional->GetColour());
		 GetShaderAt(0U)->SetVec3("u_directional.colour.specular", m_lightDirectional->GetColour());
		 GetShaderAt(0U)->SetVec4("u_directional.direction", m_lightDirectional->GetDirection());

		 // Point
		 //GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.ambient", m_lightPoint->GetColour() * 0.15f);
		 GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.diffuse", m_lightPoint->GetColour());
		 GetShaderAt(0U)->SetVec3("u_pointLights[0].colour.specular", m_lightPoint->GetColour());
		 GetShaderAt(0U)->SetVec4("u_pointLights[0].position", m_lightPoint->GetPosition());
		 GetShaderAt(0U)->SetFloat("u_pointLights[0].linear", 0.045f);
		 GetShaderAt(0U)->SetFloat("u_pointLights[0].quadratic", 0.0075f);

		 // Spot
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
		 m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices()));
		 m_shaders.get()->push_back(make_unique<Shader>("assets/shaders/light"));
		 GetMeshAt(1U)->LoadTextures(*GetShaderAt(1U));
		 GetShaderAt(1U)->SetVec3("u_colour", m_lightPoint->GetColour());
		 mat4 lightModel = mat4(1.0f);
		 lightModel = glm::translate(lightModel, vec3(m_lightPoint->GetPosition()));
		 GetShaderAt(1U)->SetMat4("u_model", (mat4)lightModel);

		 // Spot light cube
		 m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices()));
		 m_shaders.get()->push_back(make_unique<Shader>("assets/shaders/light"));
		 GetMeshAt(2U)->LoadTextures(*GetShaderAt(2U));
		 GetShaderAt(2U)->SetVec3("u_colour", m_lightSpot->GetColour());
		 lightModel = mat4(1.0f);
		 lightModel = glm::translate(lightModel, vec3(m_lightSpot->GetPosition()));
		 GetShaderAt(2U)->SetMat4("u_model", (mat4)lightModel);
		#pragma endregion
	}

	#pragma region Getters
	Mesh* Renderer::GetMeshAt(unsigned int pPos)
	{
		if (m_meshes.get() == nullptr)
			return nullptr;

		if (pPos > m_meshes.get()->size() - 1)
		{
			#ifdef _DEBUG
			 cout << "Attempting to access mesh outside array size\n";
			#endif
			return nullptr;
		}

		return (*m_meshes.get())[pPos].get();
	}

	Shader* Renderer::GetShaderAt(unsigned int pPos)
	{
		if (m_shaders.get() == nullptr)
			return nullptr;

		if (pPos > m_shaders.get()->size() - 1)
		{
			#ifdef _DEBUG
		 	 cout << "Attempting to access shader outside array size\n";
			#endif
			return nullptr;
		}

		return (*m_shaders.get())[pPos].get();
	}
	#pragma endregion
}
