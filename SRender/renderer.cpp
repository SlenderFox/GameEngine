#include "renderer.hpp"
#include "graphics.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "debug.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::string;
using std::vector;

namespace srender
{
	namespace renderer
	{
		camera *s_camera = nullptr;
		vector<model*> s_models;
		vector<light*> s_lights;

		bool init(const float _aspect) noexcept
		{
			// Default clear colour
			setClearColour({0.1f, 0.1f, 0.1f});

			// Initialise camera
			s_camera = new camera(_aspect, 75.0f);
			s_camera->setPosition({ 0.0f, 0.0f, 6.0f });

			// Initialise arrays
			s_models = vector<model*>();
			s_lights = vector<light*>();

			return true;
		}

		void terminate() noexcept
		{
			for (unsigned int i = 0; i < s_models.size(); ++i)
			{	delete s_models[i]; }

			for (unsigned int i = 0; i < s_lights.size(); ++i)
			{	delete s_lights[i]; }

			// Destroy all textures
			for (size_t i = 0; i < texture::s_loadedTextures.size(); ++i)
			{
				// For safety
				if (texture::s_loadedTextures.at(i))
					delete texture::s_loadedTextures.at(i);
			}
			// Unload all textures from gl memory once finished
			texture::unloadAll();

			delete s_camera;
		}

		void draw() noexcept
		{
			// Clears to background colour
			graphics::clearScreenBuffers();

			if (s_models.size() > 0)
			{
				for (uint8_t i = 0; i < s_models.size(); ++i)
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

			for (uint8_t i = 0; i < s_lights.size(); ++i)
			{
				light *currentLight = getLightAt(i);
				switch (currentLight->getType())
				{
				case lightType::Directional:
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
				case lightType::Point:
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
				case lightType::Spot:
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
			for (uint8_t i = 0, count = 0; i < (uint8_t)s_lights.size(); ++i)
			{
				light *currentlLight = getLightAt(i);

				// We only want to modify the spotlights, ignore the others
				if (currentlLight->getType() != lightType::Spot) continue;

				string numLights = std::to_string(count);
				float limit = _isAngle ? 90.0f : 1.0f;
				float newValue = _isAngle ? currentlLight->getAngleRaw() : currentlLight->getBlurRaw();
				newValue += _value;

				if (newValue <= limit && newValue >= 0.0f)
				{
					// Update the value in the light
					if (_isAngle)
						currentlLight->setAngle(newValue);
					else
						currentlLight->setBlur(newValue);

					// Update the shaders on all the models
					for (uint8_t j = 0; j < s_models.size(); ++j)
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
			uint8_t &_outId,
			const string *_modelPath,
			const string *_shaderPath,
			const bool _loadTextures
		) noexcept
		{
			// Caps at 255
			size_t currentAmount = s_models.size();
			if (currentAmount > 255)
				return nullptr;

			_outId = (uint8_t)currentAmount;
			s_models.push_back(new model(_modelPath, _shaderPath, s_camera, _loadTextures));
			return getModelAt(_outId);
		}

		light *addNewLight(
			uint8_t &_outId,
			const lightType _type,
			const colour _colour
		) noexcept
		{
			// Caps at 255
			size_t currentAmount = s_lights.size();
			if (currentAmount > 255)
				return nullptr;

			_outId = (uint8_t)currentAmount;
			s_lights.push_back(new light(_type, _colour));
			return getLightAt(_outId);
		}

		void setClearColour(const colour _colour) noexcept
		{
			vec3 col = _colour.rgb();
			graphics::setClearColour(col.r, col.g, col.b);
		}

		void setRenderMode(const mode _mode) noexcept
		{
			graphics::setRenderMode(int(_mode));
		}

		uint8_t modelCount() noexcept
		{
			return (uint8_t)s_models.size();
		}

		uint8_t lightCount() noexcept
		{
			return (uint8_t)s_lights.size();
		}

		model *getModelAt(const uint8_t _pos) noexcept
		{
			if (_pos > s_models.size() - 1)
			{
				debug::send("Attempting to access model outside array size");
				return nullptr;
			}

			return s_models[_pos];
		}

		light *getLightAt(const uint8_t _pos) noexcept
		{
			if (_pos > s_lights.size() - 1)
			{
				debug::send("Attempting to access light outside array size");
				return nullptr;
			}

			return s_lights[_pos];
		}

		camera *getCamera() noexcept
		{	return s_camera; }
	}
}
