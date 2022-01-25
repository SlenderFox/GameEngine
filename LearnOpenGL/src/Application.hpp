#pragma once
#include "Renderer.hpp"
#include "Input.hpp"
#include "Camera.hpp"

struct GLFWwindow;

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
        //virtual void Draw() = 0;

    private:
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

        static Application* sm_appRef;

        // TODO: Fix these variable comments
        bool m_gladLoaded = false;  // Whether glad has loaded or not
        unsigned int m_winWidth = 0, m_winHeight = 0;   // The width and height of the window
        unsigned int m_frames = 0, m_fps = 0;           // The amount of frames rendered per second
        double m_currentTime = 0, m_prevTime = 0, m_deltaTime = 0;  // The time between rendered frames
        double m_frameInterval = 0; // Accruement of time use for periodic updates
        double m_mouseLastX = 400, m_mouseLastY = 300;  // Mouse position in the last frame
        double m_yaw = 90.0f, m_pitch = 0; // The rotation of the camera

        GLFWwindow* m_window = nullptr;     // A reference to the window
        Renderer* m_rendererInst = nullptr; // A reference to the renderer instance
        Input* m_inputInst = nullptr;       // A reference to the input instance
        Camera* m_cameraRef = nullptr;      // A reference to a camera
    };
}
