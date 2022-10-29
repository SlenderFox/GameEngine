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
			Fail_Imgui,
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
		static double s_mouseLastX, s_mouseLastY, s_camYaw, s_camPitch;

		static std::string s_title;
		static ExitCode s_exitCode;

#		pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
#		pragma endregion

		static void Terminate() noexcept;

		static bool Init();
		static bool SetupGLFW();
		static bool SetupGlad();
		static bool SetupImgui();

		static void UpdateCamera() noexcept;
		static void UpdateFrameTimeData() noexcept;
		static void ProcessInput() noexcept;

	public:
		// Should evaluate to 0.01666666666666666
		static const double s_fixedDeltaTime/* = 1.0 / 60.0*/;

		static Application* const& GetApplication() noexcept { return s_application; }
		static const bool GladLoaded() noexcept;	// No definition to allow forward declaration
		static void Quit() noexcept;

		static ExitCode Run();
		static void SetDimensions(uint16_t const& pWidth, uint16_t const& pHeight) noexcept;
		static void SetTitle(std::string const& pTitle) noexcept;
		static void SetFullscreen(bool const& pFullscreen) noexcept;

		// TODO: Remove these
		static void MouseCallback(double& pPosX, double& pPosY) noexcept;
		static void ScrollCallback(double& pOffsetX, double& pOffsetY) noexcept;

		static double GetTime() noexcept { return s_currentTime; }
		static double GetDeltaTime() noexcept { return s_deltaTime; }
		static constexpr double GetFixedDeltaTime() noexcept { return s_fixedDeltaTime; }

	protected:
		Application();
		virtual ~Application();

		virtual bool Startup() = 0;
		virtual void Shutdown() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void LateUpdate() = 0;
	};
}
