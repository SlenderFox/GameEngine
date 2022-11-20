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

		#pragma region Constructors
		// Pure static class
		Renderer() = delete;
		~Renderer() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		#pragma endregion

		static bool Init(const float pAspect) noexcept;
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

		static void LoadLightsIntoShader(const Shader* pShader) noexcept;
		static void ModifyAllSpotlightAngles(const float pValue) noexcept;
		static void ModifyAllSpotlightBlurs(const float pValue) noexcept;

		static Model* AddNewModel(
			uint8_t& id,
			const std::string* pModelPath,
			const std::string* pShaderPath,
			const bool pLoadTextures = true) noexcept;
		static Light* AddNewLight(
			uint8_t& id,
			const LightType pType,
			const Colour pColour = Colour::White()) noexcept;

		static void SetClearColour(const Colour pColour) noexcept;
		static void SetRenderMode(const Mode pMode = Mode::Fill) noexcept;
		static void SetResolution(const size_t pWidth, const size_t pHeight) noexcept;

		static uint8_t ModelCount() noexcept;
		static uint8_t LightCount() noexcept;
		static Model* GetModelAt(const uint8_t pPos) noexcept;
		static Light* GetLightAt(const uint8_t pPos) noexcept;

		static constexpr Camera* GetCamera() noexcept { return s_camera; }
	};
}
