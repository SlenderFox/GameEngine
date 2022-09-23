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
	// Forward declaration
	class Application { public: static const bool GladLoaded() noexcept; };

	// Static

	const float Renderer::s_ambience = 0.15f;

	void Renderer::SetClearColour(Colour pColour)
	{
		vec3 col = pColour.RGBvec3();
		glClearColor(col.r, col.g, col.b, 1.0f);
	}

	// Member

	void Renderer::Init(float pAspect)
	{
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);

		//SetClearColour(Colour::CreateWithRGB(vec3(0.1f)));

		// Initialise camera
		m_camera = new Camera(pAspect, 75.0f);
		m_camera->SetPosition({ 0.0f, 0.0f, 6.0f });

		// Initialise arrays
		m_models = make_unique<vector<unique_ptr<Model>>>();
		m_lights = make_unique<vector<unique_ptr<Light>>>();
	}

	Renderer::~Renderer()
	{
		if (Application::GladLoaded())
		{
			if (m_models)
			{
				m_models.get()->clear();
				// Smart pointer needs to be manually released or it throws an error :|
				m_models.release();

				// Destroy all textures
				for (size_t i = 0; i < Model::s_loadedTextures.size(); ++i)
				{
					if (Model::s_loadedTextures[i])
						delete Model::s_loadedTextures[i];
				}
				// Unload all textures from memory once finished
				Texture::UnloadAll();
			}
		}

		delete m_camera;
	}

	void Renderer::Draw()
	{
		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_models.get()->size() > 0)
		{
			for (uint8_t i = 0; i < m_models.get()->size(); ++i)
				GetModelAt(i)->Draw();
		}
	}

	void Renderer::LoadLightsIntoShader(const Shader& pShader)
	{
		pShader.SetFloat("u_material.shininess", 32.0f);
		uint8_t numDirLights = 0;
		uint8_t numPointLights = 0;
		uint8_t numSpotLights = 0;

		for (uint8_t i = 0; i < m_lights.get()->size(); ++i)
		{
			Light* currentLight = GetLightAt(i);
			switch (currentLight->GetType())
			{
			case LightType::Directional:
				pShader.SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.ambient", (vec3)currentLight->GetColour() * s_ambience);
				pShader.SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.diffuse", currentLight->GetColour());
				pShader.SetVec3("u_dirLights[" + std::to_string(numDirLights) + "].colour.specular", currentLight->GetColour());
				pShader.SetVec4("u_dirLights[" + std::to_string(numDirLights) + "].direction", currentLight->GetDirection());
				++numDirLights; break;
			case LightType::Point:
				pShader.SetVec3("u_pointLights[" + std::to_string(numPointLights) + "].colour.diffuse", currentLight->GetColour());
				pShader.SetVec3("u_pointLights[" + std::to_string(numPointLights) + "].colour.specular", currentLight->GetColour());
				pShader.SetVec4("u_pointLights[" + std::to_string(numPointLights) + "].position", currentLight->GetPosition());
				pShader.SetFloat("u_pointLights[" + std::to_string(numPointLights) + "].linear", currentLight->GetLinear());
				pShader.SetFloat("u_pointLights[" + std::to_string(numPointLights) + "].quadratic", currentLight->GetQuadratic());
				++numPointLights; break;
			case LightType::Spot:
				pShader.SetVec3("u_spotLights[" + std::to_string(numSpotLights) + "].colour.diffuse", currentLight->GetColour());
				pShader.SetVec3("u_spotLights[" + std::to_string(numSpotLights) + "].colour.specular", currentLight->GetColour());
				pShader.SetVec4("u_spotLights[" + std::to_string(numSpotLights) + "].position", currentLight->GetPosition());
				pShader.SetVec4("u_spotLights[" + std::to_string(numSpotLights) + "].direction", currentLight->GetDirection());
				pShader.SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].linear", currentLight->GetLinear());
				pShader.SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].quadratic", currentLight->GetQuadratic());
				pShader.SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].cutoff", currentLight->GetAngle());
				pShader.SetFloat("u_spotLights[" + std::to_string(numSpotLights) + "].blur", currentLight->GetBlur());
				++numSpotLights; break;
			default: return;
			}
		}
	}

	void Renderer::ModifyAllSpotlightAngles(float pValue)
	{
		for (uint8_t i = 0; i < m_lights.get()->size(); ++i)
		{
			if (GetLightAt(i)->GetType() == LightType::Spot)
			{
				Light* currentlLight = GetLightAt(i);
				float newValue = currentlLight->GetAngleRaw() + pValue;
				if (newValue <= 90.0f && newValue >= 0.0f)
				{
					currentlLight->SetAngle(newValue);
					for (uint8_t i = 0; i < m_models.get()->size(); ++i)
						GetModelAt(i)->m_shader->SetFloat("u_spotLights[0].cutoff", currentlLight->GetAngle());
				}
			}
		}
	}

	void Renderer::ModifyAllSpotlightBlurs(float pValue)
	{
		for (uint8_t i = 0; i < m_lights.get()->size(); ++i)
		{
			if (GetLightAt(i)->GetType() == LightType::Spot)
			{
				Light* currentlLight = GetLightAt(i);
				float newValue = currentlLight->GetBlurRaw() + pValue;
				if (newValue <= 1.0f && newValue > 0.0f)
				{
					currentlLight->SetBlur(newValue);
					for (uint8_t i = 0; i < m_models.get()->size(); ++i)
						GetModelAt(i)->m_shader->SetFloat("u_spotLights[0].blur", currentlLight->GetBlur());
				}
			}
		}
	}

	Model* Renderer::AddNewModel(uint8_t &id, string pModelPath, std::string pShaderPath, bool pLoadTextures)
	{
		// Caps at 255
		size_t currentAmount = m_models.get()->size();
		if (currentAmount > 255)
			return nullptr;

		id = (uint8_t)currentAmount;
		m_models.get()->push_back(make_unique<Model>((char*)pModelPath.c_str(), (char*)pShaderPath.c_str(), m_camera, pLoadTextures));
		return GetModelAt(id);
	}

	Light* Renderer::AddNewLight(uint8_t &id, LightType pType, Colour pColour)
	{
		// Caps at 255
		size_t currentAmount = m_lights.get()->size();
		if (currentAmount > 255)
			return nullptr;

		id = (uint8_t)currentAmount;
		m_lights.get()->push_back(make_unique<Light>(pType, pColour));
		return GetLightAt(id);
	}

	#pragma region Getters
	uint8_t Renderer::ModelCount() const
	{
		return (uint8_t)m_models.get()->size();
	}

	uint8_t Renderer::LightCount() const
	{
		return (uint8_t)m_lights.get()->size();
	}

	Model* Renderer::GetModelAt(uint8_t pPos)
	{
		if (!m_models.get())
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

	Light* Renderer::GetLightAt(uint8_t pPos)
	{
		if (!m_lights.get())
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
