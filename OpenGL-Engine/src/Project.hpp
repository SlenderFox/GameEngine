#pragma once
#include "Application.hpp"

class Project: public Engine::Application
{
private:
	Engine::Renderer* rendererRef;

	Engine::Entity* object_lightPoint;
	Engine::Entity* object_lightSpot;
	Engine::Entity* object_backpack;
	Engine::Entity* object_cube;

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

	void CreateScene();
	void CreateLights();

public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
};
