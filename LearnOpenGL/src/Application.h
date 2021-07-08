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
	~Application();

	bool Run();

private:
	const unsigned int winWidth, winHeight;
	unsigned int m_idVAO, m_idVBO, m_idEBO;

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

	GLFWwindow* m_window = nullptr;
	Shader* m_shaderRef = nullptr;
	Texture* m_textureRef = nullptr;
    Input* m_inputRef = nullptr;
    Camera* m_cameraRef = nullptr;
};