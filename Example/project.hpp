#pragma once
#include "application.hpp"
#include "entity.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

/** Example project */
class project : public srender::application
{
	static double s_camYaw, s_camPitch;

	/** Static function to use as a callback for mouse input
	 * @param inDeltaX The horizontal position change since last call
	 * @param inDeltaY The vertical position change since last call
	 */
	static void mouseCallback(double inDeltaX, double inDeltaY) noexcept;

	/** Static function to use as a callback for scroll input
	 * @param inOffsetX The horizontal position change since last call
	 * @param inOffsetY The vertical position change since last call
	 */
	static void scrollCallback(double inOffsetX, double inOffsetY) noexcept;

	static constexpr uint8_t s_numCubes = 9U;
	static const glm::vec3 s_cubePositions[s_numCubes];

	srender::entity *m_backpack;
	std::vector<srender::entity*> m_cubes = std::vector<srender::entity*>();
	std::vector<srender::entity*> m_lights = std::vector<srender::entity*>();

	/** Loads entities in to create the scene */
	void createScene();
	/** Loads lights in to light the scene */
	void createLights();

	/** Crappy way to check input each frame */
	void processInput();
	/** Modifies a value if shift or control are pressed
	 * @param _value The input value
	 * @return [float] The result
	 */
	float valueModKeys(float _value) noexcept;

public:
	project();
	~project();

	/** Called once at the start of runtime
	 * @return [bool] False will terminate application
	 */
	_NODISCARD bool startup() override;
	/** Called when the application shuts down */
	void shutdown() override;
	/** Called once at the start of every frame */
	void update() override;
	/** Called 60 times per second, after Update */
	void fixedUpdate() override;
	/** Called once per frame, after fixedUpdate but still before rendering */
	void lateUpdate() override;
};
