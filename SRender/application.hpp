#pragma once
#include "graphics.hpp"
#include "input.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** The core of the engine, any project must inhert from this class to get started
 * @todo Alt releases cursor
 * @todo F11 toggles between fullscreen and windowed modes
 */
class application
{
	// Delete copy/move so extra instances can't be created/moved.
	application(application const&) = delete;
	application &operator=(application const&) = delete;
	application(application&&) = delete;
	application &operator=(application&&) = delete;

	_NODISCARD static bool init();
	static void loop();
	static void terminate() noexcept;

	static void setAppLocation(std::string _path) noexcept;

public:
	static void quit() noexcept;

	static void setDimensions(const uint16_t _width, const uint16_t _height) noexcept;
	static void setTitle(const std::string _title) noexcept;
	static void setFullscreen(const bool _fullscreen) noexcept;

	_NODISCARD static application *getApplication() noexcept;
	_NODISCARD static double getTime() noexcept;
	_NODISCARD static double getDeltaTime() noexcept;
	_NODISCARD static std::string getAppLocation() noexcept;
	/** Should evaluate to 0.01666666666666666 */
	_NODISCARD static constexpr double getFixedDeltaTime() { return 1.0 / 60.0; }

	/** Runs the app
	 * @param _argc The number of arguments
	 * @param _args The arguments as an array of C strings
	 * @return int Return code
	 */
	_NODISCARD int run(int _argc, char *_args[]);

protected:
	application() noexcept;
	virtual ~application() noexcept {}

	/** Called once at the start of runtime
	 * @return [bool] False will terminate application
	 */
	_NODISCARD virtual bool startup() = 0;
	/** Called when the application shuts down */
	virtual void shutdown() = 0;
	/** Called once at the start of every frame */
	virtual void update() = 0;
	/** Called 60 times per second, after Update */
	virtual void fixedUpdate() = 0;
	/** Called once per frame, after fixedUpdate but still before rendering */
	virtual void lateUpdate() = 0;
};
}
