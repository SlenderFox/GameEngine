#include "renderer.hpp"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "debug.hpp"

using glm::vec3;
using glm::mat3;
using glm::mat4;
using std::string;
using std::vector;
using std::make_unique;
using std::unique_ptr;

namespace srender
{
	camera *renderer::s_camera = nullptr;
	unique_ptr<vector<unique_ptr<model>>> renderer::s_models = nullptr;
	unique_ptr<vector<unique_ptr<light>>> renderer::s_lights = nullptr;
	const float renderer::s_ambience = 0.15f;

	bool renderer::init(const float inAspect) noexcept
	{
		// Enables the use of the depth buffer
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);

		//setClearColour(colour::CreateWithRGB(vec3(0.1f)));

		// Initialise camera
		s_camera = new camera(inAspect, 75.0f);
		s_camera->setPosition({ 0.0f, 0.0f, 6.0f });

		// Initialise arrays
		s_models = make_unique<vector<unique_ptr<model>>>();
		s_lights = make_unique<vector<unique_ptr<light>>>();

		return true;
	}

	void renderer::terminate() noexcept
	{
		if (s_models)
		{
			s_models.get()->clear();
			// Smart pointer needs to be manually released or it throws an error :|
			s_models.release();
		}

		// Destroy all textures
		for (size_t i = 0; i < texture::s_loadedTextures.size(); ++i)
		{
			// For safety
			if (texture::s_loadedTextures.at(i))
				delete texture::s_loadedTextures.at(i);
		}
		// Unload all textures from memory once finished
		texture::unloadAll();

		delete s_camera;
	}

	void renderer::draw() noexcept
	{
		// Clears to background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (s_models.get()->size() > 0)
		{
			for (uint8_t i = 0; i < s_models.get()->size(); ++i)
				getModelAt(i)->draw();
		}
	}

	void renderer::loadLightsIntoShader(const shader *inShader) noexcept
	{
		inShader->setFloat("u_material.shininess", 32.0f);
		uint8_t numDirLights = 0;
		uint8_t numPointLights = 0;
		uint8_t numSpotLights = 0;
		string lightCount;

		for (uint8_t i = 0; i < s_lights.get()->size(); ++i)
		{
			light *currentLight = getLightAt(i);
			switch (currentLight->getType())
			{
			case lightType::Directional:
				lightCount = std::to_string(numDirLights);
				inShader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.ambient",
					(vec3)currentLight->getColour() * s_ambience
				);
				inShader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.diffuse",
					currentLight->getColour()
				);
				inShader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.specular",
					currentLight->getColour()
				);
				inShader->setFloat4(
					"u_dirLights[" + lightCount + "].direction",
					currentLight->getDirection()
				);
				++numDirLights; break;
			case lightType::Point:
				lightCount = std::to_string(numPointLights);
				inShader->setFloat3(
					"u_pointLights[" + lightCount + "].colour.diffuse",
					currentLight->getColour()
				);
				inShader->setFloat3(
					"u_pointLights[" + lightCount + "].colour.specular",
					currentLight->getColour()
				);
				inShader->setFloat4(
					"u_pointLights[" + lightCount + "].position",
					currentLight->getPosition()
				);
				inShader->setFloat(
					"u_pointLights[" + lightCount + "].linear",
					currentLight->getLinear()
				);
				inShader->setFloat(
					"u_pointLights[" + lightCount + "].quadratic",
					currentLight->getQuadratic()
				);
				++numPointLights; break;
			case lightType::Spot:
				lightCount = std::to_string(numSpotLights);
				inShader->setFloat3(
					"u_spotLights[" + lightCount + "].colour.diffuse",
					currentLight->getColour()
				);
				inShader->setFloat3(
					"u_spotLights[" + lightCount + "].colour.specular",
					currentLight->getColour()
				);
				inShader->setFloat4(
					"u_spotLights[" + lightCount + "].position",
					currentLight->getPosition()
				);
				inShader->setFloat4(
					"u_spotLights[" + lightCount + "].direction",
					currentLight->getDirection()
				);
				inShader->setFloat(
					"u_spotLights[" + lightCount + "].linear",
					currentLight->getLinear()
				);
				inShader->setFloat(
					"u_spotLights[" + lightCount + "].quadratic",
					currentLight->getQuadratic()
				);
				inShader->setFloat(
					"u_spotLights[" + lightCount + "].cutoff",
					currentLight->getAngle()
				);
				inShader->setFloat(
					"u_spotLights[" + lightCount + "].blur",
					currentLight->getBlur()
				);
				++numSpotLights; break;
			default:
				debug::send("Incorrect light type"); return;
			}
		}
	}

	void renderer::modifyAllSpotlights(
		const bool inIsAngle,
		const float inValue
	) noexcept
	{
		for (uint8_t i = 0, count = 0; i < (uint8_t)s_lights.get()->size(); ++i)
		{
			light *currentlLight = getLightAt(i);

			// We only want to modify the spotlights, ignore the others
			if (currentlLight->getType() != lightType::Spot) continue;

			string numLights = std::to_string(count);
			float limit = inIsAngle ? 90.0f : 1.0f;
			float newValue = inIsAngle ? currentlLight->getAngleRaw() : currentlLight->getBlurRaw();
			newValue += inValue;

			if (newValue <= limit && newValue >= 0.0f)
			{
				// Update the value in the light
				if (inIsAngle)
					currentlLight->setAngle(newValue);
				else
					currentlLight->setBlur(newValue);

				// Update the shaders on all the models
				for (uint8_t j = 0; j < s_models.get()->size(); ++j)
				{
					if (inIsAngle)
					{
						getModelAt(j)->m_shader->setFloat(
							"u_spotLights[" + numLights + "].cutoff",
							currentlLight->getAngle()
						);
					}
					else
					{
						getModelAt(j)->m_shader->setFloat(
							"u_spotLights[" + numLights + "].blur",
							currentlLight->getBlur()
						);
					}
				}
			}
			// Only incremented for a spotlight
			++count;
		}
	}

	model *renderer::addNewModel(
		uint8_t &outId,
		const string *inModelPath,
		const string *inShaderPath,
		const bool inLoadTextures) noexcept
	{
		// Caps at 255
		size_t currentAmount = s_models.get()->size();
		if (currentAmount > 255)
			return nullptr;

		outId = (uint8_t)currentAmount;
		s_models.get()->push_back(make_unique<model>(inModelPath, inShaderPath, s_camera, inLoadTextures));
		return getModelAt(outId);
	}

	light *renderer::addNewLight(
		uint8_t &outId,
		const lightType inType,
		const colour inColour) noexcept
	{
		// Caps at 255
		size_t currentAmount = s_lights.get()->size();
		if (currentAmount > 255)
			return nullptr;

		outId = (uint8_t)currentAmount;
		s_lights.get()->push_back(make_unique<light>(inType, inColour));
		return getLightAt(outId);
	}

	void renderer::setClearColour(const colour inColour) noexcept
	{
		vec3 col = inColour.rgb();
		glClearColor(col.r, col.g, col.b, 1.0f);
	}

	void renderer::setRenderMode(const Mode inMode) noexcept
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + (int)inMode);
	}

	void renderer::setResolution(
		const size_t inWidth,
		const size_t inHeight
	) noexcept
	{
		glViewport(0, 0, (GLsizei)inWidth, (GLsizei)inHeight);
	}

	uint8_t renderer::modelCount() noexcept
	{
		return (uint8_t)s_models.get()->size();
	}

	uint8_t renderer::lightCount() noexcept
	{
		return (uint8_t)s_lights.get()->size();
	}

	model *renderer::getModelAt(const uint8_t inPos) noexcept
	{
		if (!s_models.get())
		{
			debug::send("No model vector found");
			return nullptr;
		}

		if (inPos > s_models.get()->size() - 1)
		{
			debug::send("Attempting to access model outside array size");
			return nullptr;
		}

		return (*s_models.get())[inPos].get();
	}

	light *renderer::getLightAt(const uint8_t inPos) noexcept
	{
		if (!s_lights.get())
		{
			debug::send("No light vector found");
			return nullptr;
		}

		if (inPos > s_lights.get()->size() - 1)
		{
			debug::send("Attempting to access light outside array size");
			return nullptr;
		}

		return (*s_lights.get())[inPos].get();
	}

	camera *renderer::getCamera() noexcept
	{ return s_camera; }
}
