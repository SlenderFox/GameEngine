/*Following: https://learnopengl.com/
* Enable/disable console through
* [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
*/
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WAITFORINPUT true;

using std::cout;
using std::cin;
using std::endl;

const unsigned int winWidth = 800;
const unsigned int winHeight = 600;

// Function prototypes
bool run();
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight);
void processInput(GLFWwindow* pWindow);

int main()
{
    if (!run())
    {
        // Once the window has closed keep the console open
#ifdef WAITFORINPUT
        // Waits for any input
        cout << "Press enter to close...";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cin.get();
#else
        // Waits 5 seconds
        //int closeTime = glfwGetTime() + 5;
        //while (glfwGetTime() < closeTime) { }
#endif
    }

	return 0;
}

bool run()
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

    Shader* shader = new Shader("../Assets/shaders/shader.vert", "../Assets/shaders/shader.frag");

    int vertexColourLocation = glGetUniformLocation(shader->m_ID, "shapeColour");

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

    // ----------Texture----------
    // Generates two texture objects
    unsigned int id_TEX0, id_TEX1;
    glGenTextures(1, &id_TEX0);
    glGenTextures(1, &id_TEX1);
    // Rmember this worlks like a pointer to the object using the ID
    glBindTexture(GL_TEXTURE_2D, id_TEX0);

    // Sets some parameters to the currently bound texture object
    float borderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Makes sure the images are oriented correctly when loaded
    stbi_set_flip_vertically_on_load(true);

    // Loads the first texture into memory
    int texWidth, textHeight, texColChannels;
    unsigned char* data = stbi_load("../Assets/textures/container.jpg", &texWidth, &textHeight, &texColChannels, 0);
    if (data)
    {
        /*Applies the image to the texture object and creates the mipmaps
        * p1: What object we are applying to
        * p2: Specifies which mipmap level we are applying to (0 for base)
        * p3: What format to store the texture as
        * p4/5: The width and height of the texture
        * p6: Border (legacy stuff, leave as 0)
        * p7: What format the image is
        * p8: The datatype being passed in (this this case a char)
        * p9: The actual image being passed in
        */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture0" << endl;
    }
    // Frees the image memory
    stbi_image_free(data);

    // Binds the second texture
    glBindTexture(GL_TEXTURE_2D, id_TEX1);
    // Sets some parameters to the currently bound texture object
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loads the seconds texture into memory
    data = stbi_load("../Assets/textures/awesomeface.png", &texWidth, &textHeight, &texColChannels, 0);
    if (data)
    {
        // Applies the image to the texture object and creates the mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture1" << endl;
    }
    // Frees the image memory
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_TEX0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_TEX1);

    shader->use();
    shader->setInt("texture0", 0);
    shader->setInt("texture1", 1);
    glBindVertexArray(id_VAO);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        //float time = glfwGetTime();
        //float greenValue = (sin(time) / 2.0f) + 0.5f;

        // Input
        processInput(window);

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // Not required since we only have 1 shader program and VAO
        //glUseProgram(shader->m_ID);
        //glBindVertexArray(id_VAO);

        //glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbinds the vertex array
        //glBindVertexArray(0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean-up once done
    glDeleteVertexArrays(1, &id_VAO);
    glDeleteBuffers(1, &id_VBO);
    glDeleteBuffers(1, &id_EBO);
    
    delete shader;

    glfwTerminate();
    return true;
}

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
    glViewport(0, 0, pWidth, pHeight);
}

// Handles all the input
void processInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_END) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
