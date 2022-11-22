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
		static Application* s_application;
		// Memory is managed by glfw
		static GLFWwindow* s_windowRef;

		// Whether glad has loaded or not
		static bool s_gladLoaded;

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

		static std::string s_title;
		static ExitCode s_exitCode;

		#pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
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
			GLFWwindow* pWindow,
			const int pWidth,
			const int pHeight) noexcept;

	public:
		// Should evaluate to 0.01666666666666666
		static constexpr double s_fixedDeltaTime = 1.0 / 60.0;

		static Application* GetApplication() noexcept { return s_application; }
		static const bool GladLoaded() noexcept;	// No definition to allow forward declaration
		static void Quit() noexcept;

		static ExitCode Run();
		static void SetDimensions(const uint16_t pWidth, const uint16_t pHeight) noexcept;
		static void SetTitle(const std::string pTitle) noexcept;
		static void SetFullscreen(const bool pFullscreen) noexcept;

		static double GetTime() noexcept { return s_currentTime; }
		static double GetDeltaTime() noexcept { return s_deltaTime; }

	protected:
		Application();
		virtual ~Application() {}

		virtual bool Startup() = NULL;
		virtual void Shutdown() = NULL;
		virtual void Update() = NULL;
		virtual void FixedUpdate() = NULL;
		virtual void LateUpdate() = NULL;
	};
}
