#pragma region
#pragma once
#include "Entity.hpp"
#pragma endregion

namespace Engine
{
	class Renderer
	{
		friend class Application;	// Allowing application access and control
	// Static
	public:
		static Renderer* GetInstance()
		{
			static Renderer* sm_instance = new Renderer();
			return sm_instance;
		}

		//void CreateModelScene();
		//void RenderModelScene(double pTime);
		//void CreateBoxScene();
		//void RenderBoxScene(double pTime);
		//void CreateModelLights();
		//void CreateMeshLights();

		Model* AddNewModel(unsigned int &id, string pLocation, Shader* pShaderRef = nullptr);
		Shader* AddNewShader(unsigned int &id, string pLocation);
		Light* AddNewLight(unsigned int &id, LightType pType, vec3 pColour = vec3(1));
		
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
		Mesh* GetMeshAt(unsigned int pPos);
		#pragma endregion

	// Member
	private:
		Camera* m_cameraRef = nullptr;	// A reference to a camera
		unique_ptr<vector<unique_ptr<Model>>> m_models;
		unique_ptr<vector<unique_ptr<Shader>>> m_shaders;
		unique_ptr<vector<unique_ptr<Light>>> m_lights;
		unique_ptr<vector<unique_ptr<Mesh>>> m_meshes;

		const vec3 m_cubePositions[10] = {
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
