#pragma region
#pragma once
#include "Renderer.hpp"
#include "Input.hpp"

struct GLFWwindow;
#pragma endregion

namespace Engine
{
	class Application
	{
	public:
		virtual ~Application();

		static Application* GetApplication() { return sm_appRef; }

		/**
		 * @brief The entrypoint to the application
		 *
		 * @param pWidth The desired width in pixels of the window
		 * @param pHeight The desired height in pixels of the window
		 * @param pTitle pTitle The title of the window
		 * @param pFullscreen If the window is fullscreened or windowed
		 */
		void Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen);

		/**
		 * @brief Sets the dimensions of the window and calculates aspect ratio.
		 * If the camera exists, update it aswell
		 *
		 * @param pWidth The desired width in pixels of the window
		 * @param pHeight The desired height in pixels of the window
		 */
		void SetDimensions(unsigned int pWidth, unsigned int pHeight);

		void MouseCallback(double pPosX, double pPosY);
		void ScrollCallback(double pOffsetX, double pOffsetY);

		const double m_fixedDeltaTime = 0.01666666666666666;

	protected:
		Application();

		/**
		 * @brief Called at the end of Init()
		 *
		 * @return If there was a failure during startup that requires a crash
		 */
		virtual bool Startup() = 0;
		/**
		 * @brief Called after main loop has exited and before application termination
		 */
		virtual void Shutdown() = 0;
		/**
		 * @brief Called once per frame
		 *
		 * @param pDeltaTime The time between frames
		 */
		virtual void Update(double pDeltaTime) = 0;
		/**
		 * @brief Called once every 60 seconds
		 *
		 * @param pFixedDeltaTime The time between every fixed update, should be roughly 0.0166-
		 */
		virtual void FixedUpdate(double pFixedDeltaTime) = 0;
		//virtual void LateUpdate(double pDeltaTime) = 0;
		//virtual void Draw() = 0;

	private:
		#pragma region Constructors
		// Delete copy/move so extra instances can't be created/moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		#pragma endregion

		/**
		 * @brief Called At the start of Run() for app initialisation
		 *
		 * @param pTitle The title of the window
		 * @param pFullscreen If the window is fullscreened or windowed
		 * @return If initialisation was successful
		 */
		bool Init(string pTitle, bool pFullscreen);
		/**
		 * @brief Updates the camera's aspect ratio and fov
		 */
		void UpdateCamera();
		/**
		 * @brief Temporary local input prcoessing
		 */
		void ProcessInput();

		static Application* sm_appRef;		// Static reference to the application
		GLFWwindow* m_window = nullptr;     // A reference to the window
		Renderer* m_rendererInst = nullptr; // A reference to the renderer instance
		Input* m_inputInst = nullptr;       // A reference to the input instance

		bool m_gladLoaded = false;                          // Whether glad has loaded or not
		unsigned int m_winWidth = 0U, m_winHeight = 0U;     // The width and height of the window
		unsigned int m_frames = 0U;                         // The total amount of frames rendered
		unsigned int m_fps = 0U, m_framesPerSecond = 0U;    // The amount of frames rendered per second
		double m_currentTime = 0.0,
		 m_prevTime = 0.0, m_deltaTime = 0.0;               // The time between rendered frames
		double m_fixedTimer = 0.0f, m_frameTimer = 0.0f;     // Timers used for calling fixed update and displaying fps

		double m_mouseLastX = 400.0, m_mouseLastY = 300.0;  // Mouse position in the last frame
		double m_yaw = 90.0, m_pitch = 0.0;                 // The rotation of the camera
	};
}
