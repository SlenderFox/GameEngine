#pragma once
#include "application.hpp"
#include "entity.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

/** Example project. */
namespace project
{
	/** Static function to use as a callback for mouse input.
	 * @param inDeltaX The horizontal position change since last call.
	 * @param inDeltaY The vertical position change since last call.
	 */
	void mouseCallback(double inDeltaX, double inDeltaY) noexcept;

	/** Static function to use as a callback for scroll input.
	 * @param inOffsetX The horizontal position change since last call.
	 * @param inOffsetY The vertical position change since last call.
	 */
	void scrollCallback(double inOffsetX, double inOffsetY) noexcept;

	/** Loads entities in to create the scene. */
	void createScene();
	/** Loads lights in to light the scene. */
	void createLights();

	/** Crappy way to check input each frame. */
	void processInput();
	/** Modifies a value if shift or control are pressed.
	 * @param _value The input value.
	 * @return [float] The result.
	 */
	float valueModKeys(float _value) noexcept;

	_NODISCARD int run(int _argc, char *_args[]);

	/** Called once at the start of runtime.
	 * @return [bool] False will terminate application.
	 */
	void startup();
	/** Called when the application shuts down. */
	void shutdown();
	/** Called once at the start of every frame. */
	void update();
	/** Called 60 times per second, after Update. */
	void fixedUpdate();
	/** Called once per frame, after fixedUpdate but still before rendering. */
	void lateUpdate();
};
