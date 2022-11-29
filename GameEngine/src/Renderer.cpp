#pragma region
#include "Renderer.hpp"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Debug.hpp"

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
	#pragma region Variables
	Camera* Renderer::s_camera = nullptr;
	unique_ptr<vector<unique_ptr<Model>>> Renderer::s_models = nullptr;
	unique_ptr<vector<unique_ptr<Light>>> Renderer::s_lights = nullptr;
	const float Renderer::s_ambience = 0.15f;
	#pragma endregion

	bool Renderer::Init(const float inAspect) noexcept
	{
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);

		//SetClearColour(Colour::CreateWithRGB(vec3(0.1f)));

		// Initialise camera
		s_camera = new Camera(inAspect, 75.0f);
		s_camera->SetPosition({ 0.0f, 0.0f, 6.0f });

		// Initialise arrays
		s_models = make_unique<vector<unique_ptr<Model>>>();
		s_lights = make_unique<vector<unique_ptr<Light>>>();

		return true;
	}

	void Renderer::Terminate() noexcept
	{
		if (s_models)
		{
			s_models.get()->clear();
			// Smart pointer needs to be manually released or it throws an error :|
			s_models.release();
		}

		// Destroy all textures
		for (size_t i = 0; i < Texture::s_loadedTextures.size(); ++i)
		{
			// For safety
			if (Texture::s_loadedTextures.at(i))
				delete Texture::s_loadedTextures.at(i);
		}
		// Unload all textures from memory once finished
		Texture::UnloadAll();

		delete s_camera;
	}

	void Renderer::Draw() noexcept
	{
		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (s_models.get()->size() > 0)
		{
			for (uint8_t i = 0; i < s_models.get()->size(); ++i)
				GetModelAt(i)->Draw();
		}
	}

	void Renderer::LoadLightsIntoShader(const Shader* inShader) noexcept
	{
		inShader->SetFloat("u_material.shininess", 32.0f);
		uint8_t numDirLights = 0;
		uint8_t numPointLights = 0;
		uint8_t numSpotLights = 0;
		string lightCount;

		for (uint8_t i = 0; i < s_lights.get()->size(); ++i)
		{
			Light* currentLight = GetLightAt(i);
			switch (currentLight->GetType())
			{
			case LightType::Directional:
				lightCount = std::to_string(numDirLights);
				inShader->SetVec3(
					"u_dirLights[" + lightCount + "].colour.ambient",
					(vec3)currentLight->GetColour() * s_ambience
				);
				inShader->SetVec3(
					"u_dirLights[" + lightCount + "].colour.diffuse",
					currentLight->GetColour()
				);
				inShader->SetVec3(
					"u_dirLights[" + lightCount + "].colour.specular",
					currentLight->GetColour()
				);
				inShader->SetVec4(
					"u_dirLights[" + lightCount + "].direction",
					currentLight->GetDirection()
				);
				++numDirLights; break;
			case LightType::Point:
				lightCount = std::to_string(numPointLights);
				inShader->SetVec3(
					"u_pointLights[" + lightCount + "].colour.diffuse",
					currentLight->GetColour()
				);
				inShader->SetVec3(
					"u_pointLights[" + lightCount + "].colour.specular",
					currentLight->GetColour()
				);
				inShader->SetVec4(
					"u_pointLights[" + lightCount + "].position",
					currentLight->GetPosition()
				);
				inShader->SetFloat(
					"u_pointLights[" + lightCount + "].linear",
					currentLight->GetLinear()
				);
				inShader->SetFloat(
					"u_pointLights[" + lightCount + "].quadratic",
					currentLight->GetQuadratic()
				);
				++numPointLights; break;
			case LightType::Spot:
				lightCount = std::to_string(numSpotLights);
				inShader->SetVec3(
					"u_spotLights[" + lightCount + "].colour.diffuse",
					currentLight->GetColour()
				);
				inShader->SetVec3(
					"u_spotLights[" + lightCount + "].colour.specular",
					currentLight->GetColour()
				);
				inShader->SetVec4(
					"u_spotLights[" + lightCount + "].position",
					currentLight->GetPosition()
				);
				inShader->SetVec4(
					"u_spotLights[" + lightCount + "].direction",
					currentLight->GetDirection()
				);
				inShader->SetFloat(
					"u_spotLights[" + lightCount + "].linear",
					currentLight->GetLinear()
				);
				inShader->SetFloat(
					"u_spotLights[" + lightCount + "].quadratic",
					currentLight->GetQuadratic()
				);
				inShader->SetFloat(
					"u_spotLights[" + lightCount + "].cutoff",
					currentLight->GetAngle()
				);
				inShader->SetFloat(
					"u_spotLights[" + lightCount + "].blur",
					currentLight->GetBlur()
				);
				++numSpotLights; break;
			default:
				Debug::Send("Incorrect light type"); return;
			}
		}
	}

	void Renderer::ModifyAllSpotlights(
		const bool inIsAngle,
		const float inValue
	) noexcept
	{
		for (uint8_t i = 0, count = 0; i < (uint8_t)s_lights.get()->size(); ++i)
		{
			Light* currentlLight = GetLightAt(i);

			// We only want to modify the spotlights, ignore the others
			if (currentlLight->GetType() != LightType::Spot) continue;

			string numLights = std::to_string(count);
			float limit = inIsAngle ? 90.0f : 1.0f;
			float newValue = inIsAngle ? currentlLight->GetAngleRaw() : currentlLight->GetBlurRaw();
			newValue += inValue;

			if (newValue <= limit && newValue >= 0.0f)
			{
				// Update the value in the light
				if (inIsAngle)
					currentlLight->SetAngle(newValue);
				else
					currentlLight->SetBlur(newValue);

				// Update the shaders on all the models
				for (uint8_t j = 0; j < s_models.get()->size(); ++j)
				{
					if (inIsAngle)
					{
						GetModelAt(j)->m_shader->SetFloat(
							"u_spotLights[" + numLights + "].cutoff",
							currentlLight->GetAngle()
						);
					}
					else
					{
						GetModelAt(j)->m_shader->SetFloat(
							"u_spotLights[" + numLights + "].blur",
							currentlLight->GetBlur()
						);
					}
				}
			}
			// Only incremented for a spotlight
			++count;
		}
	}

	Model* Renderer::AddNewModel(
		uint8_t& outId,
		const string* inModelPath,
		const string* inShaderPath,
		const bool inLoadTextures) noexcept
	{
		// Caps at 255
		size_t currentAmount = s_models.get()->size();
		if (currentAmount > 255)
			return nullptr;

		outId = (uint8_t)currentAmount;
		s_models.get()->push_back(make_unique<Model>(inModelPath, inShaderPath, s_camera, inLoadTextures));
		return GetModelAt(outId);
	}

	Light* Renderer::AddNewLight(
		uint8_t& outId,
		const LightType inType,
		const Colour inColour) noexcept
	{
		// Caps at 255
		size_t currentAmount = s_lights.get()->size();
		if (currentAmount > 255)
			return nullptr;

		outId = (uint8_t)currentAmount;
		s_lights.get()->push_back(make_unique<Light>(inType, inColour));
		return GetLightAt(outId);
	}

	void Renderer::SetClearColour(const Colour inColour) noexcept
	{
		vec3 col = inColour.RGBvec3();
		glClearColor(col.r, col.g, col.b, 1.0f);
	}

	void Renderer::SetRenderMode(const Mode inMode) noexcept
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + (int)inMode);
	}

	void Renderer::SetResolution(
		const size_t inWidth,
		const size_t inHeight
	) noexcept
	{
		glViewport(0, 0, (GLsizei)inWidth, (GLsizei)inHeight);
	}

	uint8_t Renderer::ModelCount() noexcept
	{
		return (uint8_t)s_models.get()->size();
	}

	uint8_t Renderer::LightCount() noexcept
	{
		return (uint8_t)s_lights.get()->size();
	}

	Model* Renderer::GetModelAt(const uint8_t inPos) noexcept
	{
		if (!s_models.get())
		{
			Debug::Send("No model vector found");
			return nullptr;
		}

		if (inPos > s_models.get()->size() - 1)
		{
			Debug::Send("Attempting to access model outside array size");
			return nullptr;
		}

		return (*s_models.get())[inPos].get();
	}

	Light* Renderer::GetLightAt(const uint8_t inPos) noexcept
	{
		if (!s_lights.get())
		{
			Debug::Send("No light vector found");
			return nullptr;
		}

		if (inPos > s_lights.get()->size() - 1)
		{
			Debug::Send("Attempting to access light outside array size");
			return nullptr;
		}

		return (*s_lights.get())[inPos].get();
	}

	Camera* Renderer::GetCamera() noexcept
	{ return s_camera; }
}
