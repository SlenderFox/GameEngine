#include "graphics.hpp"
#include "renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "exception.hpp"
#include "debug.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using std::string;
using std::vector;
using std::to_string;

namespace srender
{
namespace graphics
{
	camera *l_camera = nullptr;
	// Cannot be references for some weird vector reason
	vector<model*> l_modelRefs = vector<model*>();
	vector<light*> l_lightRefs = vector<light*>();

	bool init(const float _aspect) noexcept
	{
		// Default clear colour
		setClearColour(vec3(0.1f, 0.1f, 0.1f));

		// Initialise camera
		l_camera = new camera(_aspect, 75.0f);
		l_camera->setPosition(vec3(0.0f, 0.0f, 6.0f));

		texture::init();

		return true;
	}

	void terminate() noexcept
	{
		texture::terminate();
		delete l_camera;
	}

	void draw()
	{
		// Clears to background colour
		renderer::clearScreenBuffers();

		for (uint8_t i = 0; i < modelCount(); ++i)
		{
			model *cur = getModelAt(i);
			cur->getShaderRef()->use();
			cur->getShaderRef()->setMat4("u_camera", l_camera->getWorldToCameraMatrix());
			cur->getShaderRef()->setFloat3("u_viewPos", (vec3)l_camera->getPosition());
			cur->draw();
		}
	}

	void loadLightsIntoShader(const shader *_shader) noexcept
	{
		_shader->setFloat("u_material.shininess", 32.0f);
		uint8_t numDirLights = 0;
		uint8_t numPointLights = 0;
		uint8_t numSpotLights = 0;
		string lightNum;

		for (uint8_t i = 0; i < lightCount(); ++i)
		{
			light *currentLight = getLightAt(i);
			switch (currentLight->getType())
			{
			case light::type::directional:
				lightNum = to_string(numDirLights);
				_shader->setFloat3(
					"u_dirLights[" + lightNum + "].colour.ambient",
					currentLight->getColour() * getAmbience()
				);
				_shader->setFloat3(
					"u_dirLights[" + lightNum + "].colour.diffuse",
					currentLight->getColour()
				);
				_shader->setFloat3(
					"u_dirLights[" + lightNum + "].colour.specular",
					currentLight->getColour()
				);
				_shader->setFloat4(
					"u_dirLights[" + lightNum + "].direction",
					currentLight->getForward()
				);
				++numDirLights; break;
			case light::type::point:
				lightNum = to_string(numPointLights);
				_shader->setFloat3(
					"u_pointLights[" + lightNum + "].colour.diffuse",
					currentLight->getColour()
				);
				_shader->setFloat3(
					"u_pointLights[" + lightNum + "].colour.specular",
					currentLight->getColour()
				);
				_shader->setFloat4(
					"u_pointLights[" + lightNum + "].position",
					currentLight->getPosition()
				);
				_shader->setFloat(
					"u_pointLights[" + lightNum + "].linear",
					currentLight->getLinear()
				);
				_shader->setFloat(
					"u_pointLights[" + lightNum + "].quadratic",
					currentLight->getQuadratic()
				);
				++numPointLights; break;
			case light::type::spot:
				lightNum = to_string(numSpotLights);
				_shader->setFloat3(
					"u_spotLights[" + lightNum + "].colour.diffuse",
					currentLight->getColour()
				);
				_shader->setFloat3(
					"u_spotLights[" + lightNum + "].colour.specular",
					currentLight->getColour()
				);
				_shader->setFloat4(
					"u_spotLights[" + lightNum + "].position",
					currentLight->getPosition()
				);
				_shader->setFloat4(
					"u_spotLights[" + lightNum + "].direction",
					currentLight->getForward()
				);
				_shader->setFloat(
					"u_spotLights[" + lightNum + "].linear",
					currentLight->getLinear()
				);
				_shader->setFloat(
					"u_spotLights[" + lightNum + "].quadratic",
					currentLight->getQuadratic()
				);
				_shader->setFloat(
					"u_spotLights[" + lightNum + "].cutoff",
					currentLight->getAngle()
				);
				_shader->setFloat(
					"u_spotLights[" + lightNum + "].blur",
					currentLight->getBlur()
				);
				++numSpotLights; break;
			default:
				assert(false && "You forgot to add the light type to loadLightsIntoShader");
				return;
			}
		}
	}

	void updateAllShaders() noexcept
	{
		// Need to add the light in each of the shaders
		for (auto model : l_modelRefs)
		{	loadLightsIntoShader(model->getShaderRef()); }
	}

	void modifyAllSpotlights(
		const bool _isAngle,
		const float _value
	)
	{
		for (uint8_t i = 0, count = 0; i < (uint8_t)lightCount(); ++i)
		{
			light *currentlLight = getLightAt(i);

			// We only want to modify the spotlights, ignore the others
			if (currentlLight->getType() != light::type::spot) continue;

			string numLights = to_string(count);
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
				for (uint8_t j = 0; j < modelCount(); ++j)
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

	void addNewModel(model *_model)
	{	l_modelRefs.push_back(_model); }

	light *addNewLight(
		const light::type _type,
		const colour _colour
	)
	{
		light *newLight = new light(_type, _colour);
		l_lightRefs.push_back(newLight);
		return newLight;
	}

	void setClearColour(const colour _colour) noexcept
	{
		vec3 col = _colour.rgb();
		renderer::setClearColour(col.r, col.g, col.b);
	}

	void setRenderMode(const mode _mode) noexcept
	{	renderer::setRenderMode(int(_mode)); }

	void setRenderDepthBuffer(const bool _state) noexcept
	{
		for (auto model : l_modelRefs)
		{	model->getShaderRef()->setBool("u_depthBuffer", _state); }
	}

	uint8_t modelCount() noexcept
	{	return (uint8_t)l_modelRefs.size(); }

	uint8_t lightCount() noexcept
	{	return (uint8_t)l_lightRefs.size(); }

	model *getModelAt(const uint8_t _pos)
	{
		if (_pos > modelCount() - 1)
		{	throw graphicsException("Attempting to access model outside array size"); }

		return l_modelRefs[_pos];
	}

	light *getLightAt(const uint8_t _pos)
	{
		if (_pos > lightCount() - 1)
		{	throw graphicsException("Attempting to access light outside array size"); }

		return l_lightRefs[_pos];
	}

	camera *getCamera() noexcept
	{	return l_camera; }
}
}
