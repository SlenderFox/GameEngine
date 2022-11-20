/**
 * @file Project.hpp
 * @author SlenderFox
 * @date 2022-11-19
 * @copyright Copyright (c) 2022
 * @brief Example project header file
 */
#pragma once
#include "Application.hpp"

class Project: public Engine::Application
{
	static double s_camYaw, s_camPitch;

	/**
	 * @brief Static function to use as a callback for mouse input
	 * @param pDeltaX The horizontal position change since last call
	 * @param pDeltaY The vertical position change since last call
	 */
	static void MouseCallback(double pDeltaX, double pDeltaY) noexcept;
	/**
	 * @brief Static function to use as a callback for scroll input
	 * @param pOffsetX The horizontal position change since last call
	 * @param pOffsetY The vertical position change since last call
	 */
	static void ScrollCallback(double pOffsetX, double pOffsetY) noexcept;

private:
	static constexpr uint8_t s_numCubes = 9;

	const glm::vec3 m_cubePositions[s_numCubes] =
	{
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3( 1.3f, -2.0f,  -2.5f),
		glm::vec3( 1.5f,  2.0f,  -2.5f),
		glm::vec3( 1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};

	std::vector<Engine::Light*> m_lightRefs;
	std::vector<Engine::Entity*> m_cubes;
	Engine::Entity* object_backpack;

	/**
	 * @brief Loads entities in to create the scene
	 */
	void CreateScene();
	/**
	 * @brief Loads lights in to light the scene
	 */
	void CreateLights();

	/**
	 * @brief Crappy way to check input each frame
	 */
	void ProcessInput() noexcept;

public:
	Project();
	~Project();

	/**
	 * @brief Called once at the start of runtime
	 * @return [bool] False will terminate application
	 */
	bool Startup() override;
	/**
	 * @brief Called when the application shuts down
	 */
	void Shutdown() override;
	/**
	 * @brief Called once at the start of every frame
	 */
	void Update() override;
	/**
	 * @brief Called 60 times per second, after Update
	 */
	void FixedUpdate() override;
	/**
	 * @brief Called once per frame, after FixedUpdate but still before rendering
	 */
	void LateUpdate() override;
};
