#include "Application.h"
#include "Shader.h"
#include "Texture.h"
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    glViewport(0, 0, pWidth, pHeight);
}

Application::Application()
{
}

Application::~Application()
{
}

bool Application::run()
{
    // glfw: initialise and configure
    if (glfwInit() == GLFW_FALSE)
    {
        cout << "GLFW failed to initialise" << endl;
        return false;
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
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }

    // The colour filled into the the screen when glClear(GL_COLOR_BUFFER_BIT) is called
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


    // Creates a 2d triangles with each corner having a unique colour
    //float vertices[] =
    //{
    //    // Positions         // Colours
    //     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom right
    //    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom left
    //     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
    //};

    // Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
    float vertices[] =
    {
        // Positions          // Colours          // Texture coords
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left 
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
    };

    // How to construct the triangles using the verts
    unsigned int indices[] =
    {
        0, 1, 2,    // Triangle one
        0, 2, 3     // Triangle two
    };

    // Creates and assigns to an id the
    // Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
    unsigned int id_VAO, id_VBO, id_EBO;
    glGenVertexArrays(1, &id_VAO);
    glGenBuffers(1, &id_VBO);
    glGenBuffers(1, &id_EBO);

    // Binds the vertex array so that the VBO and EBO are neatly stored within
    glBindVertexArray(id_VAO);

    // GL_ARRAY_BUFFER effectively works like a pointer, using the id provided to point to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
    // Loads the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // This buffer stores the indices that reference the elements of the VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    // Texture stuff has been offloaded
    Texture* texture = new Texture();
    texture->LoadImages();

    // Shader stuff has been offloaded
    Shader* shader = new Shader("../Assets/shaders/shader.vert", "../Assets/shaders/shader.frag");

    shader->use();
    shader->setInt("texture0", 0);
    shader->setInt("texture1", 1);
    // Must be set to the current context
    glBindVertexArray(id_VAO);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Input
        processInput(window);

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
    }

    // Clean-up once done
    glDeleteVertexArrays(1, &id_VAO);
    glDeleteBuffers(1, &id_VBO);
    glDeleteBuffers(1, &id_EBO);

    delete shader;

    glfwTerminate();
    return true;
}

// Handles all the input
void Application::processInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_END) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}