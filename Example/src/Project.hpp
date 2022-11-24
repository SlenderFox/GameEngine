#pragma once
#include "Application.hpp"

/** Example project */
class Project: public Engine::Application
{
	static double s_camYaw, s_camPitch;

	/** Static function to use as a callback for mouse input
	 * @param pDeltaX The horizontal position change since last call
	 * @param pDeltaY The vertical position change since last call
	 */
	static void MouseCallback(double pDeltaX, double pDeltaY) noexcept;

	/** Static function to use as a callback for scroll input
	 * @param pOffsetX The horizontal position change since last call
	 * @param pOffsetY The vertical position change since last call
	 */
	static void ScrollCallback(double pOffsetX, double pOffsetY) noexcept;

	static constexpr
	uint8_t s_numCubes = 9U;

	static const
	glm::vec3 s_cubePositions[s_numCubes];

	std::vector<Engine::Light*> m_lightRefs;
	std::vector<Engine::Entity*> m_cubes;
	Engine::Entity* object_backpack;

	/** Loads entities in to create the scene */
	void CreateScene();
	/** Loads lights in to light the scene */
	void CreateLights();

	/** Crappy way to check input each frame */
	void ProcessInput() noexcept;

public:
	Project();
	~Project();

	/** Called once at the start of runtime
	 * @return [bool] False will terminate application
	 */
	bool Startup() override;
	/** Called when the application shuts down */
	void Shutdown() override;
	/** Called once at the start of every frame */
	void Update() override;
	/** Called 60 times per second, after Update */
	void FixedUpdate() override;
	/** Called once per frame, after FixedUpdate but still before rendering */
	void LateUpdate() override;
};
