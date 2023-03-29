#pragma once
#include "light.hpp"

namespace srender
{
	class renderer
	{
		friend class application;

	private:
		static camera *s_camera;

		static std::vector<model*> s_models;
		static std::vector<light*> s_lights;

		// Pure static class
		renderer() = delete;
		~renderer() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		renderer(renderer const&) = delete;
		renderer &operator=(renderer const&) = delete;
		renderer(renderer&&) = delete;
		renderer &operator=(renderer&&) = delete;

		static bool init(const float _aspect) noexcept;
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

		static void loadLightsIntoShader(const shader *_shader) noexcept;
		/** Modifies either the angle or blur of all spotlights by a value
		 * @note Max value is 90 for angle, 1 for blur, min for both is 0
		 * @param _isAngle True to modify the angle, false to modify the blur of the spotlight
		 * @param _value The amount to modify it by
		 */
		static void modifyAllSpotlights(
			const bool _isAngle,
			const float _value
		) noexcept;

		static model *addNewModel(
			uint8_t &_outId,
			const std::string *_modelPath,
			const std::string *_shaderPath,
			const bool _loadTextures = true) noexcept;

		static light *addNewLight(
			uint8_t &_outId,
			const lightType _type,
			const colour _colour = colour::white()) noexcept;

		static void setClearColour(const colour _colour) noexcept;
		static void setRenderMode(const Mode _mode = Mode::fill) noexcept;
		static void setResolution(
			const size_t _width,
			const size_t _height
		) noexcept;

		[[nodiscard]] static
		uint8_t modelCount() noexcept;
		[[nodiscard]] static
		uint8_t lightCount() noexcept;
		[[nodiscard]] static
		model *getModelAt(const uint8_t _pos) noexcept;
		[[nodiscard]] static
		light *getLightAt(const uint8_t _pos) noexcept;

		[[nodiscard]] static
		camera *getCamera() noexcept;
	};
}
