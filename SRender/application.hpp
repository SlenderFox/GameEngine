#pragma once
#include "renderer.hpp"
#include "input.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	class application
	{
		// Delete copy/move so extra instances can't be created/moved.
		application(application const&) = delete;
		application &operator=(application const&) = delete;
		application(application&&) = delete;
		application &operator=(application&&) = delete;

		static bool init();
		static void terminate() noexcept;

	public:
		_NODISCARD static int run();
		static void quit() noexcept;

		static void setDimensions(
			const uint16_t _width,
			const uint16_t _height
		) noexcept;
		static void setTitle(const std::string _title) noexcept;
		static void setFullscreen(const bool _fullscreen) noexcept;
		/** Sets a string saving the location of the application
		 * @param _location The first argument of main
		 * @note This function will remove everything after the final slash, so don't bother processing the argument.
		 */
		static void setAppLocation(std::string _location) noexcept;

		_NODISCARD static application *getApplication() noexcept;
		// Intended to be forward declared in other translation units
		_NODISCARD static const bool gladLoaded() noexcept;
		_NODISCARD static double getTime() noexcept;
		_NODISCARD static double getDeltaTime() noexcept;
		/** Should evaluate to 0.01666666666666666 */
		_NODISCARD static constexpr double getFixedDeltaTime() { return 1.0 / 60.0; }
		_NODISCARD static std::string getAppLocation() noexcept;

	protected:
		application() noexcept;
		virtual ~application() noexcept {}

		/** Called once at the start of runtime
		 * @return [bool] False will terminate application
		 */
		virtual bool startup() = 0;
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
