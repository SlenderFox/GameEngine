#pragma once
#include "Light.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;

	private:
		static Camera *s_camera;

		static std::unique_ptr<std::vector<std::unique_ptr<Model>>> s_models;
		static std::unique_ptr<std::vector<std::unique_ptr<Light>>> s_lights;

		#pragma region Constructors
		// Pure static class
		Renderer() = delete;
		~Renderer() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(Renderer const&) = delete;
		Renderer &operator=(Renderer const&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer &operator=(Renderer&&) = delete;
		#pragma endregion

		static bool Init(const float inAspect) noexcept;
		static void Draw() noexcept;

	public:
		enum class Mode: uint8_t
		{
			Point,
			Line,
			Fill
		};

		static const float s_ambience;

		static void Terminate() noexcept;

		static void LoadLightsIntoShader(const Shader *inShader) noexcept;
		/** Modifies either the angle or blur of all spotlights by a value
		 * @note Max value is 90 for angle, 1 for blur, min for both is 0
		 * @param pIsAngle True to modify the angle, false to modify the blur of the spotlight
		 * @param pValue The amount to modify it by
		 */
		static void ModifyAllSpotlights(
			const bool inIsAngle,
			const float inValue
		) noexcept;

		static Model *AddNewModel(
			uint8_t &outId,
			const std::string *inModelPath,
			const std::string *inShaderPath,
			const bool inLoadTextures = true) noexcept;

		static Light *AddNewLight(
			uint8_t &outId,
			const LightType inType,
			const Colour inColour = Colour::White()) noexcept;

		static void SetClearColour(const Colour inColour) noexcept;
		static void SetRenderMode(const Mode inMode = Mode::Fill) noexcept;
		static void SetResolution(
			const size_t inWidth,
			const size_t inHeight
		) noexcept;

		_NODISCARD static
		uint8_t ModelCount() noexcept;
		_NODISCARD static
		uint8_t LightCount() noexcept;
		_NODISCARD static
		Model *GetModelAt(const uint8_t inPos) noexcept;
		_NODISCARD static
		Light *GetLightAt(const uint8_t inPos) noexcept;

		_NODISCARD static
		Camera *GetCamera() noexcept;
	};
}
