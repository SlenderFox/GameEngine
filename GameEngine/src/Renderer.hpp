#pragma once
#include "Light.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;

		static Camera* s_camera;

		static std::unique_ptr<std::vector<std::unique_ptr<Model>>> s_models;
		static std::unique_ptr<std::vector<std::unique_ptr<Light>>> s_lights;

#		pragma region Constructors
		// Pure static class
		Renderer() = delete;
		~Renderer() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
#		pragma endregion

		static bool Init(float const& pAspect) noexcept;
		static void Draw() noexcept;

	public:
		enum class Mode: uint8_t
		{
			Point,
			Line,
			Fill
		};

		static const float s_ambience;

		static void Destroy() noexcept;

		static void LoadLightsIntoShader(Shader const& pShader) noexcept;
		static void ModifyAllSpotlightAngles(float const& pValue) noexcept;
		static void ModifyAllSpotlightBlurs(float const& pValue) noexcept;

		static Model* AddNewModel(
			uint8_t& id,
			std::string const& pModelPath,
			std::string const& pShaderPath,
			bool const& pLoadTextures = true) noexcept;
		static Light* AddNewLight(
			uint8_t& id,
			LightType const& pType,
			Colour const& pColour = Colour::White()) noexcept;

		static void SetClearColour(Colour const& pColour) noexcept;
		static void SetRenderMode(Mode const& pMode = Mode::Fill) noexcept;

		static constexpr Camera* const& GetCamera() noexcept { return s_camera; }
		static uint8_t ModelCount() noexcept;
		static uint8_t LightCount() noexcept;
		static Model* GetModelAt(uint8_t const& pPos) noexcept;
		static Light* GetLightAt(uint8_t const& pPos) noexcept;
	};
}
