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
namespace application
{
	using callbackFunc = void(*)(void);

	void quit() noexcept;

	void setDimensions(const uint16_t _width, const uint16_t _height) noexcept;
	void setTitle(const std::string _title) noexcept;
	void setFullscreen(const bool _fullscreen) noexcept;
	void setStartupCallback(callbackFunc _func) noexcept;
	void setShutdownCallback(callbackFunc _func) noexcept;
	void setUpdateCallback(callbackFunc _func) noexcept;
	void setFixedUpdateCallback(callbackFunc _func) noexcept;
	void setLateUpdateCallback(callbackFunc _func) noexcept;

	_NODISCARD double getTime() noexcept;
	_NODISCARD double getDeltaTime() noexcept;
	_NODISCARD std::string getAppLocation() noexcept;
	/** Should evaluate to 0.01666666666666666 */
	_NODISCARD constexpr double getFixedDeltaTime() { return 1.0 / 60.0; }

	/** Runs the app
	 * @param _argc The number of arguments
	 * @param _args The arguments as an array of C strings
	 * @return int Return code
	 */
	_NODISCARD int run(int _argc, char *_args[]);
};
}
