#include "Application.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using glm::mat4;

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    Engine::ApplicationHelper::GetApplication()->SetDimensions(pWidth, pHeight);
    glViewport(0, 0, pWidth, pHeight);
}

namespace Engine
{
    ApplicationHelper* ApplicationHelper::m_instance = nullptr;

    Application::Application() : m_idVAO(0), m_idVBO(0), m_idEBO(0)
    {
        // Default dimensions
        SetDimensions(800, 600);

        // Create the singletons in tandem with the application
        ApplicationHelper::Create(this);
        Input::Create();
    }

    Application::~Application()
    {
        // The singletons must be destroyed when the application closes
        ApplicationHelper::Destroy();
        Input::Destroy();

        delete m_textureRef;
        delete m_shaderRef;
        // Don't need to delete m_window as it is handled by glfwTerminate()
    }

    void Application::Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen)
    {
        SetDimensions(pWidth, pHeight);
        Init(pTitle, pFullscreen);

        m_shaderRef->Use();
        // This cursed bullshit is the result of me wanting to use pass by reference
        m_shaderRef->SetInt("texture0", 0);
        m_shaderRef->SetInt("texture1", 1);
        // Must be set to the current context
        glBindVertexArray(m_idVAO);

        const float radius = 10.0f;
        const float speed = 0.5f;

        // Render loop
        while (!glfwWindowShouldClose(m_window))
        {
            float time = (float)glfwGetTime();

            glfwPollEvents();
            // Input
            Input::GetInstance()->ProcessInput(m_window);

            // Rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float camX = (float)sin(time * speed) * radius;
            float camZ = (float)cos(time * speed) * radius;

            //m_cameraRef->SetView(glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
            m_shaderRef->SetMat4("camera", m_cameraRef->WorldToCameraMatrix());

            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            for (unsigned int i = 0; i < 10; i++)
            {
                mat4 model = mat4(1.0f);
                model = glm::translate(model, m_cubePositions[i]);
                float angle = time * 30.0f * ((i + 1) / (i * 0.2f + 1));
                model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
                m_shaderRef->SetMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Check and call events and swap the buffers
            glfwSwapBuffers(m_window);
        }

        // Clean-up once done
        glDeleteVertexArrays(1, &m_idVAO);
        glDeleteBuffers(1, &m_idVBO);
        glDeleteBuffers(1, &m_idEBO);
        glfwTerminate();

        return;
    }

    int Application::Init(string pTitle, bool pFullscreen)
    {
        // glfw: initialise and configure
        if (glfwInit() == GLFW_FALSE)
        {
            cout << "GLFW failed to initialise" << endl;
            return 1;
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
            cout << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return 2;
        }
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetWindowAspectRatio(m_window, 16, 9);

        //int* monCount = 0;
        //GLFWmonitor** monitors = glfwGetMonitors(monCount);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "Failed to initialize GLAD" << endl;
            glfwTerminate();
            return 3;
        }

        m_cameraRef = new Camera();
        m_cameraRef->SetClearColour(0.2f, 0.2f, 0.2f);
        m_shaderRef = new Shader("../Assets/shaders/shader.vert", "../Assets/shaders/shader.frag");
        m_textureRef = new Texture();
        m_textureRef->LoadImages();

        // Enables the use of the depth buffer
        glEnable(GL_DEPTH_TEST);

        // Creates and assigns to an id the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
        glGenVertexArrays(1, &m_idVAO);
        glGenBuffers(1, &m_idVBO);
        glGenBuffers(1, &m_idEBO);

        // Binds the vertex array so that the VBO and EBO are neatly stored within
        glBindVertexArray(m_idVAO);

        // GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_idVBO);
        // Loads the vertices to the VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

        // This buffer stores the indices that reference the elements of the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

        /*Tells the shader how to use the vertex data provided
        * p1: Which vertex attribute we want to configure (location = 0)
        * p2: Vertex size (vec3)
        * p3: The type of data (vec is using floats)
        * p4: Whether we want to normalise the data
        * p5: Stride, because the data is tightly packed this can be 0
        * p6: Offset, for some reason a void*
        */
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Colour attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texcoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Unbinds the vertex array
        glBindVertexArray(0);
        // Unbinds the GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbinds the GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return 0;
    }

    void Application::SetDimensions(unsigned int pWidth, unsigned int pHeight)
    {
        m_winWidth = pWidth;
        m_winHeight = pHeight;
        m_aspectRatio = (float)m_winWidth / (float)m_winHeight;
        m_iAspectRatio = (float)m_winHeight / (float)m_winWidth;
    }
}