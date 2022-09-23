#pragma once
#include "Light.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;
	public:
		static const float s_ambience;

		static Renderer* GetInstance()
		{
			static Renderer* s_instance = new Renderer();
			return s_instance;
		}

		static void SetClearColour(Colour pColour);

	private:
		Camera* m_camera = nullptr;
		std::unique_ptr<std::vector<std::unique_ptr<Model>>> m_models;
		std::unique_ptr<std::vector<std::unique_ptr<Shader>>> m_shaders;
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

		void Init(float pAspect);
		void Draw();

	public:
		Model* AddNewModel(uint8_t &id, std::string pLocation, Shader* pShaderRef = nullptr, bool pLoadTextures = true);
		Shader* AddNewShader(uint8_t &id, std::string pLocation);
		Light* AddNewLight(uint8_t &id, LightType pType, Colour pColour = Colour::White());
		
		void LoadLightsIntoShader(const Shader& pShader);

		void ModifyAllSpotlightAngles(float pValue);
		void ModifyAllSpotlightBlurs(float pValue);

		#pragma region Getters
		uint8_t ModelCount() const;
		uint8_t ShaderCount() const;
		uint8_t LightCount() const;
		Model* GetModelAt(uint8_t pPos);
		Shader* GetShaderAt(uint8_t pPos);
		Light* GetLightAt(uint8_t pPos);
		#pragma endregion
	};
}
