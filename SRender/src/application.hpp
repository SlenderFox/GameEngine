#pragma once
#include "renderer.hpp"
#include "input.hpp"

namespace srender
{
	class application
	{
		enum class exitCode: uint8_t
		{
			okay = 0,
			fail_GLFW_Init,
			fail_GLFW_Window,
			fail_Glad,
			fail_Renderer,
			fail_Input,
			fail_Startup
		};

		// Static reference to the application
		static application *s_application;
		// Memory is managed by glfw
		static GLFWwindow *s_windowRef;
		// Whether glad has loaded or not
		static bool s_gladLoaded;
		// Is the application fullscreen or not
		static bool s_fullscreen;
		// The width and height of the window, should only be modified through setDimensions()
		static uint16_t s_winWidth, s_winHeight;
		// The amount of frames rendered per second
		static uint16_t s_fps, s_perSecondFrameCount;
		// The total amount of frames rendered
		static uint64_t s_totalFrames;
		// Used for tracking the time between rendered frames
		static double s_currentTime, s_prevTime, s_deltaTime;
		// Timers used for calling fixed update and displaying fps
		static double s_fixedTimer, s_frameTimer;
		// The title of the window
		static std::string s_title;
		// The error code returned by main
		static exitCode s_exitCode;

		// Delete copy/move so extra instances can't be created/moved.
		application(application const&) = delete;
		application &operator=(application const&) = delete;
		application(application&&) = delete;
		application &operator=(application&&) = delete;

		static void terminate() noexcept;

		static bool init();
		static bool setupGLFW();
		static bool setupGlad();

		static void updateFrameTimeData() noexcept;
		static void updateCamera() noexcept;
		static void updateTitle() noexcept;
		static void processInput() noexcept;

		static void framebufferSizeCallback(
			GLFWwindow *inWindow,
			const int inWidth,
			const int inHeight
		) noexcept;

	public:
		// Should evaluate to 0.01666666666666666
		static constexpr
		double s_fixedDeltaTime = 1.0 / 60.0;

		[[nodiscard]] static
		application *getApplication() noexcept;

		[[nodiscard]] static const
		bool gladLoaded() noexcept;	// No definition to allow forward declaration

		static void quit() noexcept;

		[[nodiscard]] static
		exitCode run();

		static void setDimensions(
			const uint16_t inWidth,
			const uint16_t inHeight
		) noexcept;
		static void setTitle(const std::string inTitle) noexcept;
		static void setFullscreen(const bool inFullscreen) noexcept;

		[[nodiscard]] static
		double getTime() noexcept;
		[[nodiscard]] static
		double getDeltaTime() noexcept;

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
		virtual void fateUpdate() = 0;
	};
}
