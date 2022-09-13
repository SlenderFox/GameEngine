#pragma once
#include "Entity.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;	// Allowing application access and control
	public:
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
		~Renderer() {}
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		#pragma endregion

		void Init(float pAspect);
		void Destroy(bool pValidate);
		void Draw();

	public:
		Model* AddNewModel(unsigned int &id, std::string pLocation, Shader* pShaderRef = nullptr, bool pLoadTextures = true);
		Shader* AddNewShader(unsigned int &id, std::string pLocation);
		Light* AddNewLight(unsigned int &id, LightType pType, Colour pColour = Colour::White());
		
		void LoadLightsIntoShader(Shader* pShader);

		void ModifyAllSpotlightAngles(float pValue);
		void ModifyAllSpotlightBlurs(float pValue);

		#pragma region Getters
		unsigned int ModelCount() const;
		unsigned int ShaderCount() const;
		unsigned int LightCount() const;
		Model* GetModelAt(unsigned int pPos);
		Shader* GetShaderAt(unsigned int pPos);
		Light* GetLightAt(unsigned int pPos);
		#pragma endregion
	};
}
