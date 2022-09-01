#pragma region
#pragma once
#include "Entity.hpp"
#pragma endregion

namespace Engine
{
	class Renderer
	{
		friend class Application;	// Allowing application access and control
	public:
		static Renderer *GetInstance()
		{
			static Renderer *sm_instance = new Renderer();
			return sm_instance;
		}

	private:
		#pragma region Constructors
		Renderer() = default;
		~Renderer() {}
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		#pragma endregion

		/**
		 * @brief Initlialises the renderer
		 *
		 * @return If initlialisation was successful
		 */
		void Init(float pAspect);
		/**
		 * @brief Destroys the renderer but only if it was ever initliased
		 *
		 * @param pValidate Whether the renderer was even initialised
		 */
		void Destroy(bool pValidate);
		/**
		 * @brief Draws the scene
		 *
		 * @param pTime TEMPORARY! Used for basic shape animation
		 * @remark Only Application is able to call this function
		 */
		void Draw(double pTime);

		void CreateModelScene();
		void RenderModelScene(double pTime);
		void CreateBoxScene();
		void RenderBoxScene(double pTime);
		
		void CreateModelLights();
		void CreateMeshLights();
		void LoadShaderUniforms(Shader *pShader);

		void ModifySpotlightAngle(float pValue);
		void ModifySpotlightBlur(float pValue);

		Model *GetModelAt(unsigned int pPos);
		Shader *GetShaderAt(unsigned int pPos);
		Mesh *GetMeshAt(unsigned int pPos);

		Camera *m_cameraRef = nullptr;	// A reference to a camera
		unique_ptr<vector<unique_ptr<Model>>> m_models;
		unique_ptr<vector<unique_ptr<Shader>>> m_shaders;
		unique_ptr<vector<unique_ptr<Mesh>>> m_meshes;

		Light *m_lightDirectional = nullptr;
		Light *m_lightPoint = nullptr;
		Light *m_lightSpot = nullptr;

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
	};
}
