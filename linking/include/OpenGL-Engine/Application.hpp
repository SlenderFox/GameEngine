#pragma once
#include "Renderer.hpp"
#include "Input.hpp"

namespace Engine
{
	class Application
	{
	private:
		static Application* s_application;	// Static reference to the application
		static bool s_gladLoaded;				// Whether glad has loaded or not

	public:
		static Application* const& GetApplication() noexcept { return s_application; }
		static const bool GladLoaded() noexcept;	// No definition to allow forward declaration
		static void Quit() noexcept;
		
		static void MouseCallback(double& pPosX, double& pPosY) noexcept;
		static void ScrollCallback(double& pOffsetX, double& pOffsetY) noexcept;

	private:
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

		Renderer* m_rendererInst = nullptr;		// A reference to the renderer instance
		Input* m_inputInst = nullptr;				// A reference to the input instance
		GLFWwindow* m_window = nullptr;			// A reference to the window

		uint16_t m_winWidth = 0U, m_winHeight = 0U;	// The width and height of the window
		uint64_t m_totalFrames = 0U;						// The total amount of frames rendered
		// The amount of frames rendered per second
		uint16_t m_fps = 0U, m_perSecondFrameCount = 0U;
		// Used for tracking the time between rendered frames
		double m_currentTime = 0.0, m_prevTime = 0.0, m_deltaTime = 0.0;
		// Timers used for calling fixed update and displaying fps
		double m_fixedTimer = 0.0f, m_frameTimer = 0.0f;

		double m_mouseLastX = 0.0, m_mouseLastY = 0.0;	// Mouse position in the last frame
		double m_yaw = 90.0, m_pitch = 0.0;					// The rotation of the camera

		std::string m_title = "Title error";				// The tile of the window
		ExitCode m_exitCode = ExitCode::Okay;

#		pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
#		pragma endregion

		bool Init(const std::string& pTitle, bool pFullscreen);
		bool SetupGLFW(const std::string& pTitle, bool pFullscreen);
		bool SetupGlad();
		bool SetupImgui();

		void UpdateCamera() noexcept;
		void UpdateFrameTimeData() noexcept;
		void ProcessInput() noexcept;

	public:
		// Should evaluate to 0.01666666666666666
		const double m_fixedDeltaTime = 1.0 / 60.0;

		ExitCode Run(const uint16_t& pWidth, const uint16_t& pHeight, 
		 const std::string& pTitle, const bool& pFullscreen);
		void SetDimensions(const uint16_t& pWidth, const uint16_t& pHeight) noexcept;

		double GetTime() const noexcept { return m_currentTime; }
		double GetDeltaTime() const noexcept { return m_deltaTime; }
		constexpr double GetFixedDeltaTime() const noexcept { return m_fixedDeltaTime; }

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
