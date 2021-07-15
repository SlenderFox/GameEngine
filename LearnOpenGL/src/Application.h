#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"

struct GLFWwindow;

namespace Engine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

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

    protected:
        virtual bool Startup() = 0;
        virtual void Shutdown() = 0;
        virtual void Update(float pDeltaTime) = 0;
        virtual void Draw() = 0;

    private:
        /*@brief Is called by either start function
        * @param pTitle The title of the window
        * @param pTitle If the window is fullscreened or windowed
        * @return Return code, zero if successful and a number if error
        */
        int Init(string pTitle, bool pFullscreen);

        unsigned int m_winWidth, m_winHeight;   // The width and height of the window
        unsigned int m_idVAO, m_idVBO, m_idEBO; // The vertex attribute object, vertex buffer object, and element buffer object
        float m_aspectRatio, m_iAspectRatio;    // The aspect ratio and inverse aspect ratio

        // Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
        //const float m_vertices[32] =
        //{
        //	// Positions          // Colours          // Texture coords
        //	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left 
        //	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
        //	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
        //	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
        //};

        // Creates a cube, doesn't use EBO
        const float m_vertices[288] =
        {
            // Positions          // Colours          // Texture coords
            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f
        };

        // How to construct the triangles using the verts
        const unsigned int m_indices[6]
        {
            0, 1, 2,    // Triangle one
            0, 2, 3     // Triangle two
        };

        const glm::vec3 m_cubePositions[10] =
        {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        GLFWwindow* m_window = nullptr;     // A reference to the window
        Camera* m_cameraRef = nullptr;      // A reference to the camera
        Shader* m_shaderRef = nullptr;      // A reference to the shader
        Texture* m_textureRef = nullptr;    // A reference to the texture
    };

    // Static singleton class used to assist the application
    class ApplicationHelper
    {
        // Allow application to host this class
        friend Application;
    public:
        /*@return A reference to the singleton class
        */
        static ApplicationHelper* GetInstance() { return m_instance; }
        /*@return A reference to the host application
        */
        static Application* GetApplication() { return m_instance->m_application; }

    private:
        ApplicationHelper() {}
        ~ApplicationHelper() {}

        /*@brief Creates a new singleton instance, overwriting any previously existing one
        * @param pHost A reference to the host application class
        */
        static void Create(Application* pHost)
        {
            m_instance = new ApplicationHelper();
            m_instance->m_application = pHost;
        }
        /*@brief Destroys the singleton instance
        */
        static void Destroy() { delete m_instance; }

        static ApplicationHelper* m_instance;   // The singleton instance
        Application* m_application = nullptr;   // The reference to the host class
    };
}
