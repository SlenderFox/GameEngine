#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Camera.h"

using glm::vec3;

class Application
{
public:
	Application();
	Application(unsigned int pWidth, unsigned int pHeight);
	~Application();

    /*@brief Used as an interface to Init
    * @param pTitle The title of the window
    * @param pWidth The desired width in pixels of the window
    * @param pHeight The desired height in pixels of the window
    * @return Whether Init was successful
    */
    bool Start(string pTitle, unsigned int pWidth, unsigned int pHeight);
    /*@brief Used as an interface to Init
    * @param pTitle The title of the window
    * @return Whether Init was successful
    */
    bool Start(string pTitle);
    /*@brief The main run loop of the program
    * @return Whether the program exited correctly
    */
	bool Run();

private:
    /*@brief Is called by either start function
    * @param pTitle The title of the window
    * @return Whether the function was successful
    */
    bool Init(string pTitle);
    /*@brief Sets the dimensions of the window and calculates aspect ratio
    * @param pWidth The desired width in pixels of the window
    * @param pHeight The desired height in pixels of the window
    */
    void SetDimensions(unsigned int pWidth, unsigned int pHeight);

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

    const vec3 m_cubePositions[10] =
    {
        vec3(0.0f,  0.0f,  0.0f),
        vec3(2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
    };

	GLFWwindow* m_window = nullptr;     // A reference to the window
    Camera* m_cameraRef = nullptr;      // A reference to the camera
	Shader* m_shaderRef = nullptr;      // A reference to the shader
	Texture* m_textureRef = nullptr;    // A reference to the texture
    Input* m_inputRef = nullptr;        // A reference to the input
};