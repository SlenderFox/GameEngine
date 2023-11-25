#include "graphics.hpp"
#include "renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "debug.hpp"
#include "exception.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::string;
using std::vector;

namespace srender
{
namespace graphics
{
	camera *l_camera = nullptr;
	vector<model*> l_models;
	vector<light*> l_lights;

	bool init(const float _aspect) noexcept
	{
		// Default clear colour
		setClearColour({0.1f, 0.1f, 0.1f});

		// Initialise camera
		l_camera = new camera(_aspect, 75.0f);
		l_camera->setPosition({ 0.0f, 0.0f, 6.0f });

		// Initialise arrays
		l_models = vector<model*>();
		l_lights = vector<light*>();

		texture::init();

		return true;
	}

	void terminate() noexcept
	{
		for (unsigned int i = 0; i < l_models.size(); ++i)
		{	delete l_models[i]; }

		for (unsigned int i = 0; i < l_lights.size(); ++i)
		{	delete l_lights[i]; }

		texture::terminate();

		delete l_camera;
	}

	void draw() noexcept
	{
		// Clears to background colour
		renderer::clearScreenBuffers();

		if (l_models.size() > 0)
		{
			for (uint8_t i = 0; i < l_models.size(); ++i)
			{	getModelAt(i)->draw(); }
		}
	}

	void loadLightsIntoShader(const shader *_shader) noexcept
	{
		_shader->setFloat("u_material.shininess", 32.0f);
		uint8_t numDirLights = 0;
		uint8_t numPointLights = 0;
		uint8_t numSpotLights = 0;
		string lightCount;

		for (uint8_t i = 0; i < l_lights.size(); ++i)
		{
			light *currentLight = getLightAt(i);
			switch (currentLight->getType())
			{
			case light::type::directional:
				lightCount = std::to_string(numDirLights);
				_shader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.ambient",
					(vec3)currentLight->getColour() * getAmbience()
				);
				_shader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.diffuse",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat3(
					"u_dirLights[" + lightCount + "].colour.specular",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat4(
					"u_dirLights[" + lightCount + "].direction",
					(vec4)currentLight->getDirection()
				);
				++numDirLights; break;
			case light::type::point:
				lightCount = std::to_string(numPointLights);
				_shader->setFloat3(
					"u_pointLights[" + lightCount + "].colour.diffuse",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat3(
					"u_pointLights[" + lightCount + "].colour.specular",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat4(
					"u_pointLights[" + lightCount + "].position",
					(vec4)currentLight->getPosition()
				);
				_shader->setFloat(
					"u_pointLights[" + lightCount + "].linear",
					currentLight->getLinear()
				);
				_shader->setFloat(
					"u_pointLights[" + lightCount + "].quadratic",
					currentLight->getQuadratic()
				);
				++numPointLights; break;
			case light::type::spot:
				lightCount = std::to_string(numSpotLights);
				_shader->setFloat3(
					"u_spotLights[" + lightCount + "].colour.diffuse",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat3(
					"u_spotLights[" + lightCount + "].colour.specular",
					(vec3)currentLight->getColour()
				);
				_shader->setFloat4(
					"u_spotLights[" + lightCount + "].position",
					(vec4)currentLight->getPosition()
				);
				_shader->setFloat4(
					"u_spotLights[" + lightCount + "].direction",
					(vec4)currentLight->getDirection()
				);
				_shader->setFloat(
					"u_spotLights[" + lightCount + "].linear",
					currentLight->getLinear()
				);
				_shader->setFloat(
					"u_spotLights[" + lightCount + "].quadratic",
					currentLight->getQuadratic()
				);
				_shader->setFloat(
					"u_spotLights[" + lightCount + "].cutoff",
					currentLight->getAngle()
				);
				_shader->setFloat(
					"u_spotLights[" + lightCount + "].blur",
					currentLight->getBlur()
				);
				++numSpotLights; break;
			default:
				debug::send("Incorrect light type"); return;
			}
		}
	}

	void modifyAllSpotlights(
		const bool _isAngle,
		const float _value
	) noexcept
	{
		for (uint8_t i = 0, count = 0; i < (uint8_t)l_lights.size(); ++i)
		{
			light *currentlLight = getLightAt(i);

			// We only want to modify the spotlights, ignore the others
			if (currentlLight->getType() != light::type::spot) continue;

			string numLights = std::to_string(count);
			float limit = _isAngle ? 90.0f : 1.0f;
			float newValue = _isAngle ? currentlLight->getAngleRaw() : currentlLight->getBlurRaw();
			newValue += _value;

			if (newValue <= limit && newValue >= 0.0f)
			{
				// Update the value in the light
				if (_isAngle)
				{	currentlLight->setAngle(newValue); }
				else
				{	currentlLight->setBlur(newValue); }

				// Update the shaders on all the models
				for (uint8_t j = 0; j < l_models.size(); ++j)
				{
					if (_isAngle)
					{
						getModelAt(j)->getShaderRef()->setFloat(
							"u_spotLights[" + numLights + "].cutoff",
							currentlLight->getAngle()
						);
					}
					else
					{
						getModelAt(j)->getShaderRef()->setFloat(
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

	model *addNewModel(
		const string *_modelPath,
		const string *_shaderPath,
		const bool _loadTextures
	)
	{
		// Caps at 255
		size_t currentAmount = l_models.size();
		if (currentAmount >= 255)
		{	throw graphicsException("Reached max models"); }

		l_models.push_back(new model(_modelPath, _shaderPath, l_camera, _loadTextures));
		return getModelAt((uint8_t)currentAmount);
	}

	light *addNewLight(
		const light::type _type,
		const colour _colour
	)
	{
		// Caps at 255
		size_t currentAmount = l_lights.size();
		if (currentAmount > 255)
		{	throw graphicsException("Reached max lights"); }

		l_lights.push_back(new light(_type, _colour));
		return getLightAt((uint8_t)currentAmount);
	}

	void setClearColour(const colour _colour) noexcept
	{
		vec3 col = _colour.rgb();
		renderer::setClearColour(col.r, col.g, col.b);
	}

	void setRenderMode(const mode _mode) noexcept
	{
		renderer::setRenderMode(int(_mode));
	}

	uint8_t modelCount() noexcept
	{
		return (uint8_t)l_models.size();
	}

	uint8_t lightCount() noexcept
	{
		return (uint8_t)l_lights.size();
	}

	model *getModelAt(const uint8_t _pos) noexcept
	{
		if (_pos > l_models.size() - 1)
		{
			debug::send("Attempting to access model outside array size");
			return nullptr;
		}

		return l_models[_pos];
	}

	light *getLightAt(const uint8_t _pos) noexcept
	{
		if (_pos > l_lights.size() - 1)
		{
			debug::send("Attempting to access light outside array size");
			return nullptr;
		}

		return l_lights[_pos];
	}

	camera *getCamera() noexcept
	{	return l_camera; }
}
}
