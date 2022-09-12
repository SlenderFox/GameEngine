#pragma once
#include "Entity.hpp"

namespace Engine
{
	class Renderer
	{
		friend class Application;	// Allowing application access and control
	// Static
	public:
		static Renderer* GetInstance()
		{
			static Renderer* s_instance = new Renderer();
			return s_instance;
		}

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

	// Member
	private:
		Camera* m_cameraRef = nullptr;	// A reference to a camera
		std::unique_ptr<std::vector<std::unique_ptr<Model>>> m_models;
		std::unique_ptr<std::vector<std::unique_ptr<Shader>>> m_shaders;
		std::unique_ptr<std::vector<std::unique_ptr<Light>>> m_lights;

		const glm::vec3 m_cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

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
		void Draw(double pTime);
	};
}
