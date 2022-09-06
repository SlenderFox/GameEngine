#pragma region
#include "Renderer.hpp"
#include "glad/glad.h" // Include glad to get all the required OpenGL headers
#include "glm/gtc/matrix_transform.hpp"

#define MESH 0
#define CUBE 1
#define BACKPACK 2
#define RENDERMODE BACKPACK
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
		m_lightSpot = new Light(LightType::Spot, vec4(4.5f, 3, 4.5f, 1), vec3(-0.9f, -0.6f, -1), vec3(1.0f), 10.0f, 0.23f);

		// Initialise arrays
		m_models = make_unique<vector<unique_ptr<Model>>>();
		m_shaders = make_unique<vector<unique_ptr<Shader>>>();
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();

		#if RENDERMODE == MESH
		 CreateBoxScene();
		#else
		 CreateModelScene();
		#endif
	}

	void Renderer::Destroy(bool pValidate)
	{
		if (pValidate)
		{
			if (m_models)
			{
				// Loop though all shaders and call destroy on them, then release the "smart" pointer
				for (unsigned int i = 0; i < (*m_models.get()).size(); ++i)
				{
					if (GetModelAt(i) != nullptr)
						GetModelAt(i)->Destroy(pValidate);
				}
				// Smart pointer needs to be manually released or it throws an error :|
				m_models.release();
			}

			if (m_shaders)
			{
				// Loop though all shaders and call destroy on them, then release the "smart" pointer
				for (unsigned int i = 0; i < (*m_shaders.get()).size(); ++i)
				{
					if (GetShaderAt(i) != nullptr)
						GetShaderAt(i)->Destroy(pValidate);
				}
				// Smart pointer needs to be manually released or it throws an error :|
				m_shaders.release();
			}
			
			if (m_meshes)
			{
				// When model works this will be antiquated, works the same as shaders
				for (unsigned int i = 0; i < (*m_meshes.get()).size(); ++i)
				{
					if (GetMeshAt(i) != nullptr)
						GetMeshAt(i)->Destroy(pValidate);
				}
				// Smart pointer needs to be manually released or it throws an error :|
				m_meshes.release();
			}
			
			// Unload all textures from memory once finished
			Texture::UnloadAll(pValidate);
		}

		delete m_cameraRef;
	}

	void Renderer::Draw(double pTime)
	{
		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#if RENDERMODE == MESH
		 RenderBoxScene(pTime);
		#else
		 RenderModelScene(pTime);
		#endif
	}

	void Renderer::CreateModelScene()
	{

		unsigned int ID;
		#if RENDERMODE == BACKPACK
		 AddNewShader(ID, "assets/shaders/backpack");
		 Shader* shaderRef = GetShaderAt(ID);
		 shaderRef->SetMat4("u_model", mat4(1.0f));
	 	 shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
		 AddNewModel(m_modelID, "assets/models/backpack/backpack.obj", shaderRef);
		#else
		 AddNewShader(ID, "assets/shaders/cube");
		 Shader* shaderRef = GetShaderAt(ID);
		 shaderRef->SetMat4("u_model", mat4(1.0f));
	 	 shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
		 AddNewModel(m_modelID, "assets/models/cube/cube.obj", shaderRef);
		#endif

		LoadShaderUniforms(shaderRef);
		CreateModelLights();
	}

	void Renderer::RenderModelScene(double pTime)
	{
		for (unsigned int i = 0; i < m_models.get()->size(); ++i)
			GetModelAt(i)->Draw();
	}
	
	void Renderer::CreateBoxScene()
	{
		#ifdef _DEBUG
		 cout << "Loading Boxes" << endl;
		#endif
	
		m_shaders.get()->push_back(make_unique<Shader>("assets/shaders/cube"));
	
		vector<Texture> textures = vector<Texture>();
		#ifdef _DEBUG
		 cout << "\xC0";
		#endif
		textures.push_back(Texture("assets/textures/container2.png", TexType::diffuse));
		#ifdef _DEBUG
		 cout << "\xC0";
		#endif
		textures.push_back(Texture("assets/textures/container2_specular.png", TexType::specular));
	
		m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices(), textures));
		GetMeshAt(0U)->LoadTextures(*GetShaderAt(0U));
		LoadShaderUniforms(GetShaderAt(0U));
	
		CreateMeshLights();
	}

	void Renderer::RenderBoxScene(double pTime)
	{
		for (unsigned int i = 0; i < m_meshes.get()->size(); ++i)
		{
			glBindVertexArray(*GetMeshAt(i)->GetVAO());
			GetShaderAt(i)->Use();	// Needed for when drawing directly from mesh
			GetShaderAt(i)->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			GetShaderAt(i)->SetVec3("u_viewPos", m_cameraRef->GetPosition());
			if (i > 0)
				GetMeshAt(i)->Draw();
			else
			{
				for (unsigned int j = 0; j < 10; j++)
				{
					mat4 model = mat4(1.0f);
					model = glm::translate(model, m_cubePositions[j]);
					float angle = (float)pTime * 5.0f * ((j + 1) / (j * 0.2f + 1));
					model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
					GetShaderAt(0U)->SetMat4("u_model", (mat4)model);
					mat3 transposeInverseOfModel = mat3(glm::transpose(glm::inverse(model)));
					GetShaderAt(0U)->SetMat3("u_transposeInverseOfModel", (mat3)transposeInverseOfModel);
					GetMeshAt(i)->Draw();
				}
			}
		}
	}

	void Renderer::CreateModelLights()
	{
		// ----- Point light cube -----
		unsigned int discard;
		Shader* shaderRef = AddNewShader(m_lightPointID, "assets/shaders/light");
		shaderRef->SetVec3("u_colour", m_lightPoint->GetColour());
		shaderRef->SetMat4("u_model", glm::translate(mat4(1.0f), vec3(m_lightPoint->GetPosition())));
	 	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
		AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef);

		// ----- Spot light cube -----
		shaderRef = AddNewShader(m_lightSpotID, "assets/shaders/light");
		shaderRef->SetVec3("u_colour", m_lightSpot->GetColour());
		shaderRef->SetMat4("u_model", glm::translate(mat4(1.0f), vec3(m_lightSpot->GetPosition())));
	 	shaderRef->SetMat3("u_transposeInverseOfModel", (mat3)glm::transpose(glm::inverse(mat4(1.0f))));
		AddNewModel(discard, "assets/models/cube/cube.obj", shaderRef);
	}

	void Renderer::CreateMeshLights()
	{
		// ----- Point light cube -----
		Shader* shaderRef = AddNewShader(m_lightPointID, "assets/shaders/light");
		unsigned int meshID = m_meshes.get()->size();
		m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices()));
		GetMeshAt(meshID)->LoadTextures(*shaderRef);
		shaderRef->SetVec3("u_colour", m_lightPoint->GetColour());
		shaderRef->SetMat4("u_model", (mat4)glm::translate(mat4(1.0f), vec3(m_lightPoint->GetPosition())));

		// ----- Spot light cube -----
		shaderRef = AddNewShader(m_lightSpotID, "assets/shaders/light");
		meshID = m_meshes.get()->size();
		m_meshes.get()->push_back(make_unique<Mesh>(Mesh::GenerateVertices(), Mesh::GenerateIndices()));
		GetMeshAt(meshID)->LoadTextures(*shaderRef);
		shaderRef->SetVec3("u_colour", m_lightSpot->GetColour());
		shaderRef->SetMat4("u_model", (mat4)glm::translate(mat4(1.0f), vec3(m_lightSpot->GetPosition())));
	}

	void Renderer::LoadShaderUniforms(Shader* pShader)
	{
		pShader->SetFloat("u_material.shininess", 32.0f);
		// Directional
		pShader->SetVec3("u_directional.colour.ambient", m_lightDirectional->GetColour() * 0.15f);
		pShader->SetVec3("u_directional.colour.diffuse", m_lightDirectional->GetColour());
		pShader->SetVec3("u_directional.colour.specular", m_lightDirectional->GetColour());
		pShader->SetVec4("u_directional.direction", m_lightDirectional->GetDirection());

		// Point
		//pShader->SetVec3("u_pointLights[0].colour.ambient", m_lightPoint->GetColour() * 0.15f);
		pShader->SetVec3("u_pointLights[0].colour.diffuse", m_lightPoint->GetColour());
		pShader->SetVec3("u_pointLights[0].colour.specular", m_lightPoint->GetColour());
		pShader->SetVec4("u_pointLights[0].position", m_lightPoint->GetPosition());
		pShader->SetFloat("u_pointLights[0].linear", 0.045f);
		pShader->SetFloat("u_pointLights[0].quadratic", 0.0075f);

		// Spot
		//pShader->SetVec3("u_spotLights[0].colour.ambient", m_lightSpot->GetColour() * 0.15f);
		pShader->SetVec3("u_spotLights[0].colour.diffuse", m_lightSpot->GetColour());
		pShader->SetVec3("u_spotLights[0].colour.specular", m_lightSpot->GetColour());
		pShader->SetVec4("u_spotLights[0].position", m_lightSpot->GetPosition());
		pShader->SetVec4("u_spotLights[0].direction", m_lightSpot->GetDirection());
		pShader->SetFloat("u_spotLights[0].linear", 0.045f);
		pShader->SetFloat("u_spotLights[0].quadratic", 0.0075f);
		pShader->SetFloat("u_spotLights[0].cutoff", m_lightSpot->GetAngle());
		pShader->SetFloat("u_spotLights[0].blur", m_lightSpot->GetBlur());
	}

	void Renderer::ModifySpotlightAngle(float pValue)
	{
		float newValue = m_lightSpot->GetAngleRaw() + pValue;
		if (newValue <= 90.0f && newValue >= 0.0f)
		{
			m_lightSpot->SetAngle(newValue);
			GetShaderAt(m_modelID)->SetFloat("u_spotLights[0].cutoff", m_lightSpot->GetAngle());
		}
	}

	void Renderer::ModifySpotlightBlur(float pValue)
	{
		float newValue = m_lightSpot->GetBlurRaw() + pValue;
		if (newValue <= 1.0f && newValue > 0.0f)
		{
			m_lightSpot->SetBlur(newValue);
			GetShaderAt(m_modelID)->SetFloat("u_spotLights[0].blur", m_lightSpot->GetBlur());
		}
	}

	Model* Renderer::AddNewModel(unsigned int &id, string pLocation, Shader* pShaderRef)
	{
		id = m_models.get()->size();
		m_models.get()->push_back(make_unique<Model>((char*)pLocation.c_str(), m_cameraRef, pShaderRef));
		return GetModelAt(id);
	}

	Shader* Renderer::AddNewShader(unsigned int &id, string pLocation)
	{
		id = m_shaders.get()->size();
		m_shaders.get()->push_back(make_unique<Shader>(pLocation));
		return GetShaderAt(id);
	}

	Model* Renderer::GetModelAt(unsigned int pPos)
	{
		if (m_models.get() == nullptr)
			return nullptr;

		if (pPos > m_models.get()->size() - 1)
		{
			#ifdef _DEBUG
			 cout << "Attempting to access model outside array size\n";
			#endif
			return nullptr;
		}

		return (*m_models.get())[pPos].get();
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
}
