#pragma once
#include "Renderer.hpp"
#include "Input.hpp"

namespace Engine
{
	class Application
	{
		enum class ExitCode: uint8_t
		{
			Okay = 0,
			Fail_GLFW_Init,
			Fail_GLFW_Window,
			Fail_Glad,
			Fail_Renderer,
			Fail_Input,
			Fail_Startup
		};

		// Static reference to the application
		static Application *s_application;
		// Memory is managed by glfw
		static GLFWwindow *s_windowRef;
		// Whether glad has loaded or not
		static bool s_gladLoaded;
		// Is the application fullscreen or not
		static bool s_fullscreen;
		// The width and height of the window, should only be modified through SetDimensions()
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
		static ExitCode s_exitCode;

		#pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(Application const&) = delete;
		Application &operator=(Application const&) = delete;
		Application(Application&&) = delete;
		Application &operator=(Application&&) = delete;
		#pragma endregion

		static void Terminate() noexcept;

		static bool Init();
		static bool SetupGLFW();
		static bool SetupGlad();

		static void UpdateFrameTimeData() noexcept;
		static void UpdateCamera() noexcept;
		static void UpdateTitle() noexcept;
		static void ProcessInput() noexcept;

		static void FramebufferSizeCallback(
			GLFWwindow *inWindow,
			const int inWidth,
			const int inHeight
		) noexcept;

	public:
		// Should evaluate to 0.01666666666666666
		static constexpr
		double s_fixedDeltaTime = 1.0 / 60.0;

		_NODISCARD static
		Application *GetApplication() noexcept;

		_NODISCARD static const
		bool GladLoaded() noexcept;	// No definition to allow forward declaration

		static void Quit() noexcept;

		_NODISCARD static
		ExitCode Run();

		static void SetDimensions(
			const uint16_t inWidth,
			const uint16_t inHeight
		) noexcept;
		static void SetTitle(const std::string inTitle) noexcept;
		static void SetFullscreen(const bool inFullscreen) noexcept;

		_NODISCARD static
		double GetTime() noexcept;
		_NODISCARD static
		double GetDeltaTime() noexcept;

	protected:
		Application() noexcept;
		virtual ~Application() noexcept {}

		/** Called once at the start of runtime
		 * @return [bool] False will terminate application
		 */
		virtual bool Startup() = NULL;
		/** Called when the application shuts down */
		virtual void Shutdown() = NULL;
		/** Called once at the start of every frame */
		virtual void Update() = NULL;
		/** Called 60 times per second, after Update */
		virtual void FixedUpdate() = NULL;
		/** Called once per frame, after FixedUpdate but still before rendering */
		virtual void LateUpdate() = NULL;
	};
}
