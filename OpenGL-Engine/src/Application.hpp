#pragma region
#pragma once
#include "Renderer.hpp"
#include "Input.hpp"
#pragma endregion

namespace Engine
{
	class Application
	{
	// Static
	private:
		static Application* sm_appRef;		// Static reference to the application

	public:
		static Application* GetApplication() { return sm_appRef; }

	// Member
	private:
		Renderer* m_rendererInst = nullptr;					// A reference to the renderer instance
		Input* m_inputInst = nullptr;						// A reference to the input instance
		GLFWwindow* m_window = nullptr;						// A reference to the window

		bool m_gladLoaded = false;							// Whether glad has loaded or not
		unsigned int m_winWidth = 0U, m_winHeight = 0U;		// The width and height of the window
		unsigned int m_frames = 0U;							// The total amount of frames rendered
		unsigned int m_fps = 0U, m_framesPerSecond = 0U;	// The amount of frames rendered per second
		double m_currentTime = 0.0,
		 m_prevTime = 0.0, m_deltaTime = 0.0;				// The time between rendered frames
		double m_fixedTimer = 0.0f, m_frameTimer = 0.0f;	// Timers used for calling fixed update and displaying fps

		double m_mouseLastX = 400.0, m_mouseLastY = 300.0;	// Mouse position in the last frame
		double m_yaw = 90.0, m_pitch = 0.0;					// The rotation of the camera

		string m_title = "Title error";						// The tile of the window

		#pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		#pragma endregion

		bool Init(string pTitle, bool pFullscreen);
		void UpdateCamera();
		void ProcessInput();

	public:
		const double m_fixedDeltaTime = 0.01666666666666666;

		void Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen);
		void SetDimensions(unsigned int pWidth, unsigned int pHeight);
		void MouseCallback(double pPosX, double pPosY);
		void ScrollCallback(double pOffsetX, double pOffsetY);

	protected:
		Application();
		virtual ~Application();

		virtual bool Startup() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(double pDeltaTime) = 0;
		virtual void FixedUpdate(double pFixedDeltaTime) = 0;
		virtual void LateUpdate(double pDeltaTime) = 0;
	};
}
