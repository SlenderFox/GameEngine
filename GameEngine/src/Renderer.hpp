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
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
#		pragma endregion

		static bool Init(float pAspect) noexcept;
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

		static void SetClearColour(const Colour& pColour) noexcept;
		static void SetRenderMode(Mode pMode = Mode::Fill) noexcept;

		static void LoadLightsIntoShader(const Shader& pShader) noexcept;

		static void ModifyAllSpotlightAngles(float pValue) noexcept;
		static void ModifyAllSpotlightBlurs(float pValue) noexcept;

		static Model* AddNewModel(uint8_t& id, std::string pModelPath,
		 std::string pShaderPath, bool pLoadTextures = true) noexcept;
		static Light* AddNewLight(uint8_t& id, const LightType& pType,
		 const Colour& pColour = Colour::White()) noexcept;

#		pragma region Getters
		static constexpr Camera* const& GetCamera() noexcept { return s_camera; }
		static uint8_t ModelCount() noexcept;
		static uint8_t LightCount() noexcept;
		static Model* GetModelAt(uint8_t pPos) noexcept;
		static Light* GetLightAt(uint8_t pPos) noexcept;
#		pragma endregion
	};
}
