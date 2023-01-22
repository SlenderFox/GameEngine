#pragma once
#include "light.hpp"

namespace srender
{
	class renderer
	{
		friend class application;

	private:
		static camera *s_camera;

		static std::unique_ptr<std::vector<std::unique_ptr<model>>> s_models;
		static std::unique_ptr<std::vector<std::unique_ptr<light>>> s_lights;

		// Pure static class
		renderer() = delete;
		~renderer() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		renderer(renderer const&) = delete;
		renderer &operator=(renderer const&) = delete;
		renderer(renderer&&) = delete;
		renderer &operator=(renderer&&) = delete;

		static bool init(const float inAspect) noexcept;
		static void draw() noexcept;

	public:
		enum class Mode: uint8_t
		{
			point,
			line,
			fill
		};

		static const float s_ambience;

		static void terminate() noexcept;

		static void loadLightsIntoShader(const shader *inShader) noexcept;
		/** Modifies either the angle or blur of all spotlights by a value
		 * @note Max value is 90 for angle, 1 for blur, min for both is 0
		 * @param pIsAngle True to modify the angle, false to modify the blur of the spotlight
		 * @param pValue The amount to modify it by
		 */
		static void modifyAllSpotlights(
			const bool inIsAngle,
			const float inValue
		) noexcept;

		static model *addNewModel(
			uint8_t &outId,
			const std::string *inModelPath,
			const std::string *inShaderPath,
			const bool inLoadTextures = true) noexcept;

		static light *addNewLight(
			uint8_t &outId,
			const lightType inType,
			const colour inColour = colour::white()) noexcept;

		static void setClearColour(const colour inColour) noexcept;
		static void setRenderMode(const Mode inMode = Mode::fill) noexcept;
		static void setResolution(
			const size_t inWidth,
			const size_t inHeight
		) noexcept;

		_NODISCARD static
		uint8_t modelCount() noexcept;
		_NODISCARD static
		uint8_t lightCount() noexcept;
		_NODISCARD static
		model *getModelAt(const uint8_t inPos) noexcept;
		_NODISCARD static
		light *getLightAt(const uint8_t inPos) noexcept;

		_NODISCARD static
		camera *getCamera() noexcept;
	};
}
