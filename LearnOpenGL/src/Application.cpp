#include "Application.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef _DEBUG
 #include <iostream>
 #include <Windows.h>
 using std::cout;
 using std::endl;
#endif

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    Engine::Application::GetApplication()->SetDimensions(pWidth, pHeight);
    glViewport(0, 0, pWidth, pHeight);
}

namespace Engine
{
    Application* Application::sm_appRef = nullptr;

    Application::Application()
    {
        // Applies the static reference
        sm_appRef = this;

        m_rendererInst = Renderer::GetInstance();
        m_inputInst = Input::GetInstance();

        // Default dimensions
        SetDimensions(800, 600);
    }

    Application::~Application()
    {
        m_rendererInst->Destroy(m_gladLoaded);
        delete m_rendererInst;
        delete m_inputInst;
        delete m_cameraRef;
        // Don't need to delete m_window as it is handled by glfwTerminate()
    }

    void Application::Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen)
    {
        SetDimensions(pWidth, pHeight);

        if (Init(pTitle, pFullscreen))
        {
            m_prevTime = glfwGetTime();

            const float radius = 10.0f;
            const float speed = 0.5f;

            // Render loop
            while (!glfwWindowShouldClose(m_window))
            {
                m_prevTime = m_currentTime;
                m_currentTime = glfwGetTime();
                m_deltaTime = m_currentTime - m_prevTime;

                m_frames++;
                m_frameInterval += m_deltaTime;
                if (m_frameInterval >= 1.0f)
                {
                    m_fps = m_frames;
                    m_frames = 0;
                    m_frameInterval -= 1.0f;
                }

                // Input
                glfwPollEvents();
                Input::GetInstance()->ProcessInput(m_window);

                Update(m_deltaTime);

                //unsigned short cast = (unsigned short)m_frameInterval * 100;
                if (((unsigned short)(m_frameInterval * 100)) % 5 == 0)
                    FixedUpdate(m_deltaTime);

                // Skip if minimised
                if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
                    continue;

                float camX = (float)sin(m_currentTime * speed) * radius;
                float camZ = (float)cos(m_currentTime * speed) * radius;

                m_cameraRef->SetView(glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

                m_rendererInst->Draw(m_cameraRef->WorldToCameraMatrix(), m_currentTime);

                // Check and call events and swap the buffers
                glfwSwapBuffers(m_window);
            }
        }

        Shutdown();
        glfwTerminate();

        return;
    }

    bool Application::Init(string pTitle, bool pFullscreen)
    {
#ifdef _DEBUG
        // Moves the console window
        MoveWindow(GetConsoleWindow(), 10, 10, 600, 600, TRUE);
#endif
        
        // glfw: initialise and configure
        if (glfwInit() == GLFW_FALSE)
        {
#ifdef _DEBUG
            cout << "GLFW failed to initialise" << endl;
#endif
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Already set by default
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        //glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        //glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        //glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

        // glfw window creation
        m_window = glfwCreateWindow(m_winWidth, m_winHeight, pTitle.c_str(), (pFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
        if (m_window == nullptr)
        {
#ifdef _DEBUG
            cout << "Failed to create GLFW window" << endl;
#endif
            return false;
        }
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        //glfwSetWindowAspectRatio(m_window, 16, 9);

        //int* monCount = 0;
        //GLFWmonitor** monitors = glfwGetMonitors(monCount);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
#ifdef _DEBUG
            cout << "Failed to initialize GLAD" << endl;
#endif
            return false;
        }
        m_gladLoaded = true;

        if (!m_rendererInst->Init())
            return false;

        m_cameraRef = new Camera(m_aspectRatio, m_invAspectRatio, glm::radians(75.0f));
        m_cameraRef->SetClearColour(0.2f, 0.2f, 0.2f);
        m_cameraRef->SetPosition({ 0.0f, 0.0f, -6.0f });

        if (!Startup())
            return false;

        return true;
    }

    void Application::SetDimensions(unsigned int pWidth, unsigned int pHeight)
    {
        m_winWidth = pWidth;
        m_winHeight = pHeight;
        m_aspectRatio = (float)m_winWidth / (float)m_winHeight;
        m_invAspectRatio = (float)m_winHeight / (float)m_winWidth;
        if (m_cameraRef != nullptr)
            m_cameraRef->UpdateFovV(m_aspectRatio, m_invAspectRatio);
    }
}
