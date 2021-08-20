#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Camera.h"

struct GLFWwindow;

namespace Engine
{
    class Application
    {
    public:
        virtual ~Application();

        static Application* GetApplication() { return sm_appRef; }

        /*@brief The entrypoint to the application
        * @param pTitle The title of the window
        * @param pTitle If the window is fullscreened or windowed
        * @param pWidth The desired width in pixels of the window
        * @param pHeight The desired height in pixels of the window
        * @return Return code, zero if successful and a number if error
        */
        void Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen);
        /*@brief Sets the dimensions of the window and calculates aspect ratio
        * @param pWidth The desired width in pixels of the window
        * @param pHeight The desired height in pixels of the window
        */
        void SetDimensions(unsigned int pWidth, unsigned int pHeight);

        void MouseCallback(double pPosX, double pPosY);
        void ScrollCallback(double pOffsetX, double pOffsetY);

    protected:
        Application();

        virtual bool Startup() = 0;
        virtual void Shutdown() = 0;
        virtual void Update(double pDeltaTime) = 0;
        virtual void FixedUpdate(double pDeltaTime) = 0;
        //virtual void Draw() = 0;

    private:
        /*@brief Is called by either start function
        * @param pTitle The title of the window
        * @param pTitle If the window is fullscreened or windowed
        * @return If initialisation was successful
        */
        bool Init(string pTitle, bool pFullscreen);
        /*@brief First confirms camera exists then updates it's aspect ratio and fov
        */
        void UpdateCamera();

        /*@brief Temporary local input prcoessing
        */
        void ProcessInput();

        static Application* sm_appRef;

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
