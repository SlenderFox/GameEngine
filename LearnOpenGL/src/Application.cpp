#include "Application.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::cout;
using std::cin;
using std::endl;
using glm::mat4;
using glm::vec3;

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    glViewport(0, 0, pWidth, pHeight);
}

Application::Application() : winWidth(800), winHeight(600), m_idVAO(0), m_idVBO(0), m_idEBO(0)
{
    // glfw: initialise and configure
    if (glfwInit() == GLFW_FALSE)
    {
        cout << "GLFW failed to initialise" << endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Already set by default
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    //glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    //glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    //glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    // glfw window creation
    m_window = glfwCreateWindow(winWidth, winHeight, "OpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return;
    }

    // Texture stuff has been offloaded
    m_textureRef = new Texture();
    m_textureRef->LoadImages();

    // Shader stuff has been offloaded
    m_shaderRef = new Shader("../Assets/shaders/shader.vert", "../Assets/shaders/shader.frag");

    // The colour filled into the the screen when glClear(GL_COLOR_BUFFER_BIT) is called
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    /*// Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
    float m_vertices[32] =
    {
        // Positions          // Colours          // Texture coords
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
    };
    // How to construct the triangles using the verts
    const unsigned int m_indices[6]
    {
        0, 1, 2,    // Triangle one
        0, 2, 3     // Triangle two
    };*/

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
}

Application::~Application()
{
    delete m_textureRef;
    delete m_shaderRef;
    delete m_window;
}

bool Application::run()
{
    m_shaderRef->use();
    m_shaderRef->setInt("texture0", 0);
    m_shaderRef->setInt("texture1", 1);
    // Must be set to the current context
    glBindVertexArray(m_idVAO);

    // Model matrix
    mat4 model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    int modelLoc = glGetUniformLocation(m_shaderRef->m_idProgram, "model");
    // View matrix
    mat4 view = mat4(1.0f);
    view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));
    int viewLoc = glGetUniformLocation(m_shaderRef->m_idProgram, "view");
    // Perspective matrix
    mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    int projectionLoc = glGetUniformLocation(m_shaderRef->m_idProgram, "projection");

    // Render loop
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        // Input
        processInput(m_window);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(m_window);
    }

    // Clean-up once done
    glDeleteVertexArrays(1, &m_idVAO);
    glDeleteBuffers(1, &m_idVBO);
    glDeleteBuffers(1, &m_idEBO);

    glfwTerminate();
    return true;
}

// Handles all the input TODO: offload to Input class
void Application::processInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_END) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}