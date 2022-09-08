#pragma region
#include "Renderer.hpp"
#include "glad/glad.h" // Include glad to get all the required OpenGL headers
#include "glm/gtc/matrix_transform.hpp"

#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

using glm::vec3;
using glm::mat3;
using glm::mat4;
using std::string;
using std::vector;
using std::make_unique;
using std::unique_ptr;
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

		// Initialise arrays
		m_models = make_unique<vector<unique_ptr<Model>>>();
		m_shaders = make_unique<vector<unique_ptr<Shader>>>();
		m_lights = make_unique<vector<unique_ptr<Light>>>();
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

			// Destroy all textures before meshes
			for (unsigned int i = 0; i < Model::s_loadedTextures.size(); ++i)
			{
				if (Model::s_loadedTextures[i] != nullptr)
					delete Model::s_loadedTextures[i];
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
			
			// Unload all textures from memory once finished
			Texture::UnloadAll(pValidate);
		}

		delete m_cameraRef;
	}

	void Renderer::Draw(double pTime)
	{
		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_models.get()->size() > 0)
		{
			for (unsigned int i = 0; i < m_models.get()->size(); ++i)
				GetModelAt(i)->Draw();
		}
	}

	void Renderer::LoadLightsIntoShader(Shader* pShader)
	{
		pShader->SetFloat("u_material.shininess", 32.0f);
		unsigned int numDirLights = 0;
		unsigned int numPointLights = 0;
		unsigned int numSpotLights = 0;

		for (unsigned int i = 0; i < m_lights.get()->size(); ++i)
		{
			Light* currentLight = GetLightAt(i);
			switch (currentLight->GetType())
			{
			case LightType::Directional:
				pShader->SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.ambient", currentLight->GetColour() * 0.15f);
				pShader->SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.diffuse", currentLight->GetColour());
				pShader->SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.specular", currentLight->GetColour());
				pShader->SetVec4("u_dirLights[" + std::to_string(numDirLights) + "].direction", currentLight->GetDirection());
				++numDirLights; break;
			case LightType::Point:
				pShader->SetVec3("u_pointLights[" + std::to_string(numPointLights) + "].colour.diffuse", currentLight->GetColour());
				pShader->SetVec3("u_pointLights[" + std::to_string(numPointLights) + "].colour.specular", currentLight->GetColour());
				pShader->SetVec4("u_pointLights[" + std::to_string(numPointLights) + "].position", currentLight->GetPosition());
				pShader->SetFloat("u_pointLights[" + std::to_string(numPointLights) + "].linear", currentLight->GetLinear());
				pShader->SetFloat("u_pointLights[" + std::to_string(numPointLights) + "].quadratic", currentLight->GetQuadratic());
				++numPointLights; break;
			case LightType::Spot:
				pShader->SetVec3("u_spotLights[" + std::to_string(numSpotLights) + "].colour.diffuse", currentLight->GetColour());
				pShader->SetVec3("u_spotLights[" + std::to_string(numSpotLights) + "].colour.specular", currentLight->GetColour());
				pShader->SetVec4("u_spotLights[" + std::to_string(numSpotLights) + "].position", currentLight->GetPosition());
				pShader->SetVec4("u_spotLights[" + std::to_string(numSpotLights) + "].direction", currentLight->GetDirection());
				pShader->SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].linear", currentLight->GetLinear());
				pShader->SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].quadratic", currentLight->GetQuadratic());
				pShader->SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].cutoff", currentLight->GetAngle());
				pShader->SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].blur", currentLight->GetBlur());
				++numSpotLights; break;
			default: return;
			}
		}
	}

	void Renderer::ModifyAllSpotlightAngles(float pValue)
	{
		for (unsigned int i = 0; i < m_lights.get()->size(); ++i)
		{
			if (GetLightAt(i)->GetType() == LightType::Spot)
			{
				Light* currentlLight = GetLightAt(i);
				float newValue = currentlLight->GetAngleRaw() + pValue;
				if (newValue <= 90.0f && newValue >= 0.0f)
				{
					currentlLight->SetAngle(newValue);
					for (unsigned int i = 0; i < m_models.get()->size(); ++i)
						GetModelAt(i)->m_shaderRef->SetFloat("u_spotLights[0].cutoff", currentlLight->GetAngle());
				}
			}
		}
	}

	void Renderer::ModifyAllSpotlightBlurs(float pValue)
	{
		for (unsigned int i = 0; i < m_lights.get()->size(); ++i)
		{
			if (GetLightAt(i)->GetType() == LightType::Spot)
			{
				Light* currentlLight = GetLightAt(i);
				float newValue = currentlLight->GetBlurRaw() + pValue;
				if (newValue <= 1.0f && newValue > 0.0f)
				{
					currentlLight->SetBlur(newValue);
					for (unsigned int i = 0; i < m_models.get()->size(); ++i)
						GetModelAt(i)->m_shaderRef->SetFloat("u_spotLights[0].blur", currentlLight->GetBlur());
				}
			}
		}
	}

	Model* Renderer::AddNewModel(unsigned int &id, string pLocation, Shader* pShaderRef)
	{
		id = (unsigned int)m_models.get()->size();
		m_models.get()->push_back(make_unique<Model>((char*)pLocation.c_str(), pShaderRef, m_cameraRef));
		return GetModelAt(id);
	}

	Shader* Renderer::AddNewShader(unsigned int &id, string pLocation)
	{
		id = (unsigned int)m_shaders.get()->size();
		m_shaders.get()->push_back(make_unique<Shader>(pLocation));
		return GetShaderAt(id);
	}

	Light* Renderer::AddNewLight(unsigned int &id, LightType pType, vec3 pColour)
	{
		id = (unsigned int)m_lights.get()->size();
		m_lights.get()->push_back(make_unique<Light>(pType, pColour));
		return GetLightAt(id);
	}

	#pragma region Getters
	unsigned int Renderer::ModelCount() const
	{
		return (unsigned int)m_models.get()->size();
	}

	unsigned int Renderer::ShaderCount() const
	{
		return (unsigned int)m_shaders.get()->size();
	}

	unsigned int Renderer::LightCount() const
	{
		return (unsigned int)m_lights.get()->size();
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

	Light* Renderer::GetLightAt(unsigned int pPos)
	{
		if (m_lights.get() == nullptr)
			return nullptr;

		if (pPos > m_lights.get()->size() - 1)
		{
			#ifdef _DEBUG
			 cout << "Attempting to access light outside array size\n";
			#endif
			return nullptr;
		}

		return (*m_lights.get())[pPos].get();
	}
	#pragma endregion
}
