#pragma once
#include "light.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	namespace renderer
	{
		enum class mode: uint8_t
		{
			point,
			line,
			fill
		};

		bool init(const float _aspect) noexcept;
		void draw() noexcept;

		void terminate() noexcept;

		void loadLightsIntoShader(const shader *_shader) noexcept;
		/** Modifies either the angle or blur of all spotlights by a value
		 * @note Max value is 90 for angle, 1 for blur, min for both is 0
		 * @param _isAngle True to modify the angle, false to modify the blur of the spotlight
		 * @param _value The amount to modify it by
		 */
		void modifyAllSpotlights(
			const bool _isAngle,
			const float _value
		) noexcept;

		model *addNewModel(
			uint8_t &_outId,
			const std::string *_modelPath,
			const std::string *_shaderPath,
			const bool _loadTextures = true) noexcept;

		light *addNewLight(
			uint8_t &_outId,
			const lightType _type,
			const colour _colour = colour::white()) noexcept;

		void setClearColour(const colour _colour) noexcept;
		void setRenderMode(const mode _mode = mode::fill) noexcept;

		_NODISCARD uint8_t modelCount() noexcept;
		_NODISCARD uint8_t lightCount() noexcept;
		_NODISCARD model *getModelAt(const uint8_t _pos) noexcept;
		_NODISCARD light *getLightAt(const uint8_t _pos) noexcept;
		_NODISCARD camera *getCamera() noexcept;

		_NODISCARD constexpr float getAmbience() { return 0.15f; }
	}
}
