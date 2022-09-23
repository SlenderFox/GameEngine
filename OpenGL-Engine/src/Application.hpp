#pragma once
#include "Renderer.hpp"
#include "Input.hpp"

namespace Engine
{
	class Application
	{
	private:
		static Application* s_application;		// Static reference to the application

	public:
		static Application* GetApplication() { return s_application; }

	private:
		Renderer* m_rendererInst = nullptr;					// A reference to the renderer instance
		Input* m_inputInst = nullptr;						// A reference to the input instance
		GLFWwindow* m_window = nullptr;						// A reference to the window

		bool m_gladLoaded = false;							// Whether glad has loaded or not
		uint16_t m_winWidth = 0U, m_winHeight = 0U;			// The width and height of the window
		uint64_t m_frames = 0U;								// The total amount of frames rendered
		uint16_t m_fps = 0U, m_framesPerSecond = 0U;		// The amount of frames rendered per second
		double m_currentTime = 0.0,
		 m_prevTime = 0.0, m_deltaTime = 0.0;				// The time between rendered frames
		double m_fixedTimer = 0.0f, m_frameTimer = 0.0f;	// Timers used for calling fixed update and displaying fps

		double m_mouseLastX = 400.0, m_mouseLastY = 300.0;	// Mouse position in the last frame
		double m_yaw = 90.0, m_pitch = 0.0;					// The rotation of the camera

		std::string m_title = "Title error";				// The tile of the window

		#pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		#pragma endregion

		bool Init(std::string pTitle, bool pFullscreen);
		void UpdateCamera();
		void ProcessInput();

	public:
		const double m_fixedDeltaTime = 0.01666666666666666;

		void Run(uint16_t pWidth, uint16_t pHeight, std::string pTitle, bool pFullscreen);
		void SetDimensions(uint16_t pWidth, uint16_t pHeight);
		void MouseCallback(double pPosX, double pPosY);
		void ScrollCallback(double pOffsetX, double pOffsetY);

		double GetTime() const { return m_currentTime; }
		double GetDeltaTime() const { return m_deltaTime; }
		double GetFixedDeltaTime() const { return m_fixedDeltaTime; }

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
