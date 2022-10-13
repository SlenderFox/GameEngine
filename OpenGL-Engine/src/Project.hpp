#pragma once
#include "Application.hpp"

class Project: public Engine::Application
{
private:
	static const uint8_t s_numCubes = 9;

	const glm::vec3 m_cubePositions[s_numCubes] = {
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

	// Local reference to the renderer
	Engine::Renderer* m_rendererRef;
	Engine::Input* m_inputRef;

	std::vector<Engine::Light*> m_lightRefs;
	std::vector<Engine::Entity*> m_cubes;
	Engine::Entity* object_backpack;

	void CreateScene();
	void CreateLights();

	void ProcessInput() noexcept;

public:
	Project();
	~Project();

	bool Startup() override;
	void Shutdown() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
};
