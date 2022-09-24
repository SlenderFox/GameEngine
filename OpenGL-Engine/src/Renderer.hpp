#pragma once
#include "Light.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;
	public:
		static const float s_ambience;

		static Renderer* GetInstance() noexcept
		{
			static Renderer* s_instance = new Renderer();
			return s_instance;
		}

		static void SetClearColour(Colour pColour) noexcept;

	private:
		Camera* m_camera = nullptr;
		std::unique_ptr<std::vector<std::unique_ptr<Model>>> m_models;
		std::unique_ptr<std::vector<std::unique_ptr<Light>>> m_lights;

		#pragma region Constructors
		Renderer() = default;
		~Renderer();
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		#pragma endregion

		void Init(float pAspect) noexcept;
		void Draw() noexcept;

	public:
		void LoadLightsIntoShader(const Shader& pShader) noexcept;

		void ModifyAllSpotlightAngles(float pValue) noexcept;
		void ModifyAllSpotlightBlurs(float pValue) noexcept;

		Model* AddNewModel(uint8_t &id, std::string pModelPath, std::string pShaderPath, bool pLoadTextures = true) noexcept;
		Light* AddNewLight(uint8_t &id, LightType pType, Colour pColour = Colour::White()) noexcept;

		#pragma region Getters
		uint8_t ModelCount() const noexcept;
		uint8_t LightCount() const noexcept;
		Model* GetModelAt(uint8_t pPos) noexcept;
		Light* GetLightAt(uint8_t pPos) noexcept;
		#pragma endregion
	};
}
